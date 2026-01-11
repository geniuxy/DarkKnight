// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/DkInventoryPlayerController.h"

#include "DarkKnightDebugHelper.h"
#include "DkGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Components/DkItemComponent.h"
#include "DarkKnight/DarkKnight.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Subsytems/DkUISubsystem.h"
#include "Widgets/DkWidgetPrimaryLayout.h"
#include "Widgets/GameHUD/DkWidgetPickUpBox.h"
#include "Widgets/Inventory/DkWidgetInventoryScreen.h"

ADkInventoryPlayerController::ADkInventoryPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADkInventoryPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TraceForItem();
}

void ADkInventoryPlayerController::OnLoadingScreenDeactivated_Implementation()
{
	if (!IsLocalController()) return;
	PrimaryLayoutWidget = CreateWidget<UDkWidgetPrimaryLayout>(this, PrimaryLayoutClass);
	if (IsValid(PrimaryLayoutWidget))
	{
		PrimaryLayoutWidget->AddToViewport();
	}

	UDkUISubsystem* UISubsystem = UDkUISubsystem::Get(this);
	checkf(UISubsystem, TEXT("UISubsystem为空！"));
	UISubsystem->RegisterCreatedPrimaryLayoutWidget(PrimaryLayoutWidget);
	
	UISubsystem->PushSoftWidgetToStackAsync(
		DkGameplayTags::Dk_WidgetStack_GameHud,
		UDkUIFunctionLibrary::GetUISoftWidgetClassByTag(DkGameplayTags::Dk_Widget_GameHUD),
		[this](EAsyncPushWidgetState InPushState, UDkWidgetActivatableBase* PushedWidget)
		{
		}
	);

	UISubsystem->PushSoftWidgetToStackAsync(
		DkGameplayTags::Dk_WidgetStack_Interact,
		UDkUIFunctionLibrary::GetUISoftWidgetClassByTag(DkGameplayTags::Dk_Widget_PickUpBox),
		[this](EAsyncPushWidgetState InPushState, UDkWidgetActivatableBase* PushedWidget)
		{
			switch (InPushState)
			{
			case EAsyncPushWidgetState::OnCreatedBeforePush:
				{
					PushedWidget->SetOwningPlayer(this);
					FInputModeGameOnly InputModeData;
					SetInputMode(InputModeData);
					break;
				}
			case EAsyncPushWidgetState::AfterPush:
				CachedPickUpBox = CastChecked<UDkWidgetPickUpBox>(PushedWidget);
				break;
			}
		}
	);
}

void ADkInventoryPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(InputSubsystem))
	{
		InputSubsystem->AddMappingContext(IMCInventory, 0);
	}
}

void ADkInventoryPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::OnInteract);
}

void ADkInventoryPlayerController::OnInteract()
{
	Debug::Print(TEXT("我正在交互！"));
}

void ADkInventoryPlayerController::TraceForItem()
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport)) return;
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	const FVector2D ViewportCenter = ViewportSize / 2.f;
	FVector TraceStart;
	FVector TraceForward;
	if (!UGameplayStatics::DeprojectScreenToWorld(this, ViewportCenter, TraceStart, TraceForward))
	{
		return;
	}

	const FVector TraceEnd = TraceStart + TraceForward * TraceLength;
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Item);

	LastActor = ThisActor;
	ThisActor = HitResult.GetActor();

	if (!ThisActor.IsValid())
	{
		if (IsValid(CachedPickUpBox))
		{
			CachedPickUpBox->HidePickUpMessage();
		}
	}

	if (ThisActor == LastActor) return;

	if (ThisActor.IsValid())
	{
		UDkItemComponent* ItemComponent = ThisActor->FindComponentByClass<UDkItemComponent>();
		if (!IsValid(ItemComponent)) return;

		if (IsValid(CachedPickUpBox))
		{
			CachedPickUpBox->ShowPickUpMessage(ItemComponent->GetPickUpMessage());
		}
	}

	if (LastActor.IsValid())
	{
		Debug::Print(TEXT("结束定位之前的Item"));
	}
}
