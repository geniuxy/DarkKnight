// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/DkGamePlayerController.h"

#include "DarkKnightDebugHelper.h"
#include "DkGameplayTags.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Characters/DkCharacterHero.h"
#include "Characters/NPC/DkCharacterNPC.h"
#include "Components/DkActionComponent.h"
#include "Components/DkDialogComponent.h"
#include "Components/DkEnhancedInputComponent.h"
#include "Components/DkInventoryComponent.h"
#include "Components/DkItemComponent.h"
#include "DarkKnight/DarkKnight.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Interfaces/HighlightInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Subsytems/DkInventorySubsystem.h"
#include "Subsytems/DkUISubsystem.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Widgets/DkWidgetPrimaryLayout.h"
#include "Widgets/Interact/DkWidgetInteractScreen.h"
#include "Widgets/Interact/DkWidgetPickUpBox.h"

ADkGamePlayerController::ADkGamePlayerController()
{
	PrimaryActorTick.bCanEverTick = true;

	TraceLength = 500.f;
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
		UDkUIFunctionLibrary::GetUISoftWidgetClassByTag(DkGameplayTags::Dk_Widget_InteractScreen),
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
				CachedInteractScreen = CastChecked<UDkWidgetInteractScreen>(PushedWidget);
				break;
			}
		}
	);
}

void ADkGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ADkGamePlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);

	OwningPlayerCharacter = Cast<ADkCharacterBase>(NewPawn);
	if (OwningPlayerCharacter)
	{
		OwningPlayerCharacter->ServerSideInit();
		OwningPlayerCharacter->SetGenericTeamId(TeamID);
	}

	RefreshInventoryComponent();
}

void ADkGamePlayerController::AcknowledgePossession(APawn* NewPawn)
{
	Super::AcknowledgePossession(NewPawn);

	OwningPlayerCharacter = Cast<ADkCharacterBase>(NewPawn);
	if (OwningPlayerCharacter)
	{
		OwningPlayerCharacter->ClientSideInit();
	}

	RefreshInventoryComponent();
}

// void ADkGamePlayerController::OnRep_Pawn()
// {
// 	Super::OnRep_Pawn();
// 	if (!GetPawn()) return;
//
// 	RefreshInventoryComponent();
// }

void ADkGamePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	checkf(InputConfigDataAsset, TEXT("忘记配置InputConfigDataAsset了！"));

	UDkEnhancedInputComponent* EnhancedInputComponent = CastChecked<UDkEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindNativeInputAction(
		InputConfigDataAsset, DkGameplayTags::Dk_Input_Action_Move, ETriggerEvent::Triggered,
		this, &ThisClass::HandleGroundMovementInput
	);
	EnhancedInputComponent->BindNativeInputAction(
		InputConfigDataAsset, DkGameplayTags::Dk_Input_Action_ToggleMoveStyle, ETriggerEvent::Started,
		this, &ThisClass::ToggleMovementStyle
	);
	EnhancedInputComponent->BindNativeInputAction(
		InputConfigDataAsset, DkGameplayTags::Dk_Input_Action_Look, ETriggerEvent::Triggered,
		this, &ThisClass::OnLookTriggered
	);
	EnhancedInputComponent->BindNativeInputAction(
		InputConfigDataAsset, DkGameplayTags::Dk_Input_Action_Jump, ETriggerEvent::Started,
		this, &ThisClass::OnJumpPressed
	);
	EnhancedInputComponent->BindNativeInputAction(
		InputConfigDataAsset, DkGameplayTags::Dk_Input_Action_Interact, ETriggerEvent::Started,
		this, &ThisClass::OnInteract
	);
	EnhancedInputComponent->BindNativeInputAction(
		InputConfigDataAsset, DkGameplayTags::Dk_Input_Action_OpenSystemMenu, ETriggerEvent::Completed,
		this, &ThisClass::OnOpenSystemMenu
	);
	EnhancedInputComponent->BindNativeInputAction(
		InputConfigDataAsset, DkGameplayTags::Dk_Input_Action_OpenInventory, ETriggerEvent::Completed,
		this, &ThisClass::OnInventoryActionTriggered
	);
}

void ADkGamePlayerController::HandleGroundMovementInput(const FInputActionValue& InputActionValue)
{
	const FVector2D MoveVector = InputActionValue.Get<FVector2D>();

	// 获取角色控制器的偏航角（Yaw），并将其转换为一个旋转值（FRotator）。
	// 偏航角是角色在水平方向上的旋转角度，用于确定角色的朝向
	const FRotator ControllerRotation(0.f, GetControlRotation().Yaw, 0.f);

	if (MoveVector.Y != 0.f)
	{
		// ControllerRotation.RotateVector() 被用来将标准方向向量（如 FVector::ForwardVector 或 FVector::RightVector）旋转到角色的目标朝向。
		const FVector TargetForwardVector = ControllerRotation.RotateVector(FVector::ForwardVector);
		GetPawn()->AddMovementInput(TargetForwardVector, MoveVector.Y);
	}

	if (MoveVector.X != 0.f)
	{
		const FVector TargetRightVector = ControllerRotation.RotateVector(FVector::RightVector);
		GetPawn()->AddMovementInput(TargetRightVector, MoveVector.X);
	}
}

void ADkGamePlayerController::ToggleMovementStyle()
{
	ADkCharacterHero* OwningCharacter = Cast<ADkCharacterHero>(GetCharacter());
	// if (OwningCharacter->GetCurrentLocomotionStyle() == ELocomotionStyle::Walk)
	// {
	// 	OwningCharacter->SwitchLocomotionStyle(ELocomotionStyle::Run);
	// }
	// else if (OwningCharacter->GetCurrentLocomotionStyle() == ELocomotionStyle::Run)
	// {
	// 	OwningCharacter->SwitchLocomotionStyle(ELocomotionStyle::Walk);
	// }
}

void ADkGamePlayerController::OnLookTriggered(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	if (LookAxisVector.Y != 0.f)
	{
		GetPawn()->AddControllerPitchInput(LookAxisVector.Y);
	}

	if (LookAxisVector.X != 0.f)
	{
		GetPawn()->AddControllerYawInput(LookAxisVector.X);
	}
}

void ADkGamePlayerController::OnJumpPressed()
{
	ADkCharacterBase* OwningCharacter = Cast<ADkCharacterBase>(GetCharacter());
	UDkActionComponent* ActionComponent = OwningCharacter->FindComponentByClass<UDkActionComponent>();
	bool bInCombatState = ActionComponent->IsInCombatState();
	if (!bInCombatState)
	{
		OwningCharacter->Jump();
	}
}

void ADkGamePlayerController::OnInteract()
{
	if (ThisActor.IsValid())
	{
		UDkItemComponent* ItemComponent = ThisActor->FindComponentByClass<UDkItemComponent>();
		if (IsValid(ItemComponent) && InventoryComponent.IsValid())
		{
			InventoryComponent->TryAddItem(ItemComponent);
			return;
		}
	}
	if (IsValid(GetInteractiveNPC()) && IsValid(GetInteractiveNPC()->GetDialogComponent()))
	{
		UDkDialogComponent* DialogComponent = GetInteractiveNPC()->GetDialogComponent();
		if (DialogComponent->CanStartDialog())
		{
			ClientSetCameraFade(true, FColor(ForceInit), FVector2D(0.f, 1.f), 0.5f, false, true);

			HideLowerWidgetStack(DkGameplayTags::Dk_WidgetStack_Interact);
			
			FTimerHandle CameraFadeHandle;
			GetWorldTimerManager().SetTimer(CameraFadeHandle, FTimerDelegate::CreateLambda([this]()
			{
				ClientSetCameraFade(true, FColor(ForceInit), FVector2D(1.f, 0.f), 0.5f);
			}), 0.5f, false);

			FTimerHandle StartDialogHandle;
			GetWorldTimerManager().SetTimer(StartDialogHandle, FTimerDelegate::CreateLambda([=, this]()
			{
				DialogComponent->TryStartDialog(this);
			}), 1.f, false);
		}
	}
}

void ADkGamePlayerController::OnOpenSystemMenu()
{
	if (!IsLocalController()) return;

	UDkUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
		DkGameplayTags::Dk_WidgetStack_GameMenu,
		UDkUIFunctionLibrary::GetUISoftWidgetClassByTag(DkGameplayTags::Dk_Widget_SystemMenu),
		[this](EAsyncPushWidgetState InPushState, UDkWidgetActivatableBase* PushedWidget)
		{
			if (InPushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::UIOnly);
			}
			if (InPushState == EAsyncPushWidgetState::AfterPush)
			{
			}
		}
	);
}

void ADkGamePlayerController::OnInventoryActionTriggered()
{
	checkf(InventoryComponent.IsValid(), TEXT("打开库存失败，InventoryComponent未有效"));

	Debug::Print(TEXT("我正在打开库存！"));

	InventoryComponent->ConstructInventoryMenu();
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
		if (IsValid(CachedInteractScreen))
		{
			CachedInteractScreen->HidePickUpMessage();
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

		if (IsValid(CachedInteractScreen))
		{
			CachedInteractScreen->ShowPickUpMessage(ItemComponent->GetPickUpMessage());
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

void ADkGamePlayerController::RefreshInventoryComponent()
{
	// 从PlayerController获取Character是通过GetPawn() (永远不要在 BeginPlay 里假设 Pawn 已准备好!）
	InventoryComponent = CastChecked<ADkCharacterHero>(GetPawn())->FindComponentByClass<UDkInventoryComponent>();
	UDkInventorySubsystem* InventorySubsystem = UDkInventorySubsystem::Get(this);
	checkf(InventorySubsystem, TEXT("InventorySubsystem为空！"));
	InventorySubsystem->RegisterCachedInventoryComponent(InventoryComponent.Get());
}

void ADkGamePlayerController::HideLowerWidgetStack(FGameplayTag InWidgetStackTag)
{
	EWidgetStackType WidgetStackType = UDkUIFunctionLibrary::GetWidgetStackTypeByTag(InWidgetStackTag);
	for (int i = (int)EWidgetStackType::Num - 1; i > (int)WidgetStackType; --i)
	{
		FGameplayTag LowerWidgetStackTag = UDkUIFunctionLibrary::GetWidgetStackTagByType((EWidgetStackType)i);
		UCommonActivatableWidgetContainerBase* LowerWidgetStack = PrimaryLayoutWidget->FindWidgetStackByTag(LowerWidgetStackTag);
		LowerWidgetStack->SetVisibility(ESlateVisibility::Collapsed);
	}
}
