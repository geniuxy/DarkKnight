// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/DkGamePlayerController.h"

#include "DarkKnightDebugHelper.h"
#include "DkGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Components/DkInventoryComponent.h"
#include "Components/DkItemComponent.h"
#include "DarkKnight/DarkKnight.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Interfaces/HighlightInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Subsytems/DkUISubsystem.h"
#include "Widgets/DkWidgetPrimaryLayout.h"
#include "Widgets/GameHUD/DkWidgetPickUpBox.h"
#include "Widgets/Inventory/DkWidgetInventoryMenu.h"

ADkGamePlayerController::ADkGamePlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ADkGamePlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TraceForItem();
}

void ADkGamePlayerController::OnLoadingScreenDeactivated_Implementation()
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
					break;
				}
			case EAsyncPushWidgetState::AfterPush:
				CachedPickUpBox = CastChecked<UDkWidgetPickUpBox>(PushedWidget);
				break;
			}
		}
	);
}

void ADkGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(InputSubsystem))
	{
		InputSubsystem->AddMappingContext(IMCGamePlay, 0);
	}
}

void ADkGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::OnInteract);
	EnhancedInputComponent->BindAction(
		InventoryAction, ETriggerEvent::Completed, this, &ThisClass::OnInventoryActionTriggered
	);
}

void ADkGamePlayerController::OnInteract()
{
	Debug::Print(TEXT("我正在交互！"));
}

void ADkGamePlayerController::TraceForItem()
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
		if (UActorComponent* HighlightableActorComponent =
			ThisActor->FindComponentByInterface(UHighlightInterface::StaticClass()))
		{
			if (IsValid(HighlightableActorComponent))
			{
				IHighlightInterface::Execute_Highlight(HighlightableActorComponent);
			}
		}

		UDkItemComponent* ItemComponent = ThisActor->FindComponentByClass<UDkItemComponent>();
		if (!IsValid(ItemComponent)) return;

		if (IsValid(CachedPickUpBox))
		{
			CachedPickUpBox->ShowPickUpMessage(ItemComponent->GetPickUpMessage());
		}
	}

	if (LastActor.IsValid())
	{
		if (UActorComponent* HighlightableActorComponent =
			LastActor->FindComponentByInterface(UHighlightInterface::StaticClass()))
		{
			if (IsValid(HighlightableActorComponent))
			{
				IHighlightInterface::Execute_UnHighlight(HighlightableActorComponent);
			}
		}
	}
}

void ADkGamePlayerController::OnInventoryActionTriggered()
{
	UDkInventoryComponent* InventoryComponent = GetPawn()->FindComponentByClass<UDkInventoryComponent>();
	checkf(IsValid(InventoryComponent), TEXT("打开库存失败，InventoryComponent未有效"));

	Debug::Print(TEXT("我正在打开库存！"));

	InventoryComponent->ConstructInventoryMenu();
}
