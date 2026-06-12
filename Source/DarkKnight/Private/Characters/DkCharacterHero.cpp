// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DkCharacterHero.h"

#include "DarkKnightDebugHelper.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DkEquipmentComponent.h"
#include "Components/DkInventoryComponent.h"
#include "DataAssets/CharacterInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/DkAbilitySystemComponent.h"
#include "GAS/DkHeroAttributeSet.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerStates/DkPlayerStateBase.h"


class UEnhancedInputLocalPlayerSubsystem;
class ADkPlayerStateBase;

ADkCharacterHero::ADkCharacterHero()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = CameraBoomDefaultArmLength;
	CameraBoom->SocketOffset = FVector(0.f, 80.f, 50.f);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	InventoryComponent = CreateDefaultSubobject<UDkInventoryComponent>(TEXT("DkInventoryComponent"));
	EquipmentComponent = CreateDefaultSubobject<UDkEquipmentComponent>(TEXT("DkEquipmentComponent"));

	LandedDelegate.AddDynamic(this, &ThisClass::HandleOnLanded);

	HeroAttributeSet = CreateDefaultSubobject<UDkHeroAttributeSet>("HeroAttributeSet");
}

void ADkCharacterHero::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();
}

void ADkCharacterHero::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

void ADkCharacterHero::PawnClientRestart()
{
	Super::PawnClientRestart();

	APlayerController* OwningPlayerController = GetController<APlayerController>();
	if (OwningPlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
			OwningPlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (InputSubsystem)
		{
			InputSubsystem->RemoveMappingContext(AbilitySystemComponent->GetInputMappingContext());
			InputSubsystem->AddMappingContext(AbilitySystemComponent->GetInputMappingContext(), 0);
		}
	}
}

void ADkCharacterHero::SwitchLocomotionStyle(ELocomotionStyle InStyle)
{
	checkf(AttributeSet, TEXT("该Character类没有配置AttributeSet"));
	float MoveSpeedPercent = AttributeSet->GetMoveSpeed();

	SetCurrentLocomotionStyle(InStyle);

	checkf(CharacterInfo, TEXT("该Character类没有配置CharacterInfo"));
	switch (InStyle)
	{
	case ELocomotionStyle::Walk:
		GetCharacterMovement()->MaxWalkSpeed = CharacterInfo->MaxWalkSpeed * MoveSpeedPercent;
		break;
	case ELocomotionStyle::Run:
		GetCharacterMovement()->MaxWalkSpeed = CharacterInfo->MaxRunSpeed * MoveSpeedPercent;
		break;
	case ELocomotionStyle::Sprint:
		GetCharacterMovement()->MaxWalkSpeed = CharacterInfo->MaxSprintSpeed * MoveSpeedPercent;
		break;
	case ELocomotionStyle::Unknown:
		break;
	default:
		break;
	}

	Debug::Print(TEXT("行走速度"), GetCharacterMovement()->MaxWalkSpeed);
}

void ADkCharacterHero::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (PrevMovementMode == MOVE_Walking) // TODO: 并且没有骑上马
	{
		if (GetCharacterMovement()->MovementMode == MOVE_Falling)
		{
			LastJumpStartPosition = GetActorLocation();
		}

		if (GetCharacterMovement()->MovementMode == MOVE_Falling || GetCharacterMovement()->MovementMode == MOVE_Flying)
		{
			EActionState CurrentActionState = ActionComponent->GetCurrentActionState();
			if (CurrentActionState == EActionState::OutOfCombat || CurrentActionState == EActionState::InCombat)
			{
				ActionComponent->SetLastActionStateWhenOnGround(CurrentActionState);
			}
			else if (CurrentActionState == EActionState::InUIInteractionLoot ||
				CurrentActionState == EActionState::InUIInteractionMountHorse ||
				CurrentActionState == EActionState::Riding)
			{
				ActionComponent->SetLastActionStateWhenOnGround(EActionState::OutOfCombat);
			}

			ActionComponent->SetCurrentActionState(EActionState::InAir);
		}
	}
}

void ADkCharacterHero::BeginPlay()
{
	Super::BeginPlay();

	// 对于服务端的对象在BeginPlay后，设为行走状态
	// 对于Client的对象在OnRep_PlayerState中，设为行走状态
	// if (HasAuthority() && IsLocallyControlled())
	// {
	// 	SwitchLocomotionStyle(ELocomotionStyle::Walk);
	// }

	// FString NetModeStr;
	// switch (GetNetMode())
	// {
	// case NM_Standalone: NetModeStr = "Standalone";
	// 	break;
	// case NM_DedicatedServer: NetModeStr = "DedicatedServer";
	// 	break;
	// case NM_ListenServer: NetModeStr = "ListenServer";
	// 	break;
	// case NM_Client: NetModeStr = "Client";
	// 	break;
	// }
	//
	// Debug::Print(FString::Printf(
	// 		TEXT("[%s] HasAuthority: %s, IsLocallyControlled: %s, NetMode: %s"),
	// 		*GetName(),
	// 		HasAuthority() ? TEXT("True") : TEXT("False"),
	// 		IsLocallyControlled() ? TEXT("True") : TEXT("False"),
	// 		*NetModeStr
	// 	)
	// );
}

void ADkCharacterHero::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bIsLockingTarget || !IsValid(AbilitySystemComponent->GetLockTarget())) return;

	CameraFaceLockingTarget(DeltaSeconds);
}

void ADkCharacterHero::InitAbilityActorInfo()
{
	OwningPlayerState = GetPlayerState<ADkPlayerStateBase>();
}

void ADkCharacterHero::HandleOnLanded(const FHitResult& Hit)
{
	HandleFallDeath();

	// checkf(OwningPlayerState, TEXT("在着陆时，PlayerState得到为空"));
	if (!OwningPlayerState) return;
	EActionState CurrentActionState = OwningPlayerState->GetCurrentActionState();
	if (CurrentActionState == EActionState::InAir)
	{
		if (ActionComponent)
		{
			// 着落后，将ActionState设置为跳跃前的ActionState
			ActionComponent->SetCurrentActionState(ActionComponent->GetLastActionStateWhenOnGround());
		}
	}
}

void ADkCharacterHero::HandleFallDeath()
{
	// 跳跃高度过高
	if (LastJumpStartPosition.Z - GetActorLocation().Z >= DeadlyFallHeight)
	{
		// 彻底禁用角色移动组件
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->StopMovementImmediately();

		// 关闭胶囊体的碰撞，防止它和Mesh冲突
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 设置Mesh物理
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		if (BodyArmorMesh) BodyArmorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (ArmArmorMesh) ArmArmorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (BottomArmorMesh) BottomArmorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (HelmetMesh) HelmetMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		GetMesh()->SetSimulatePhysics(true);

		// 解除Mesh和胶囊体的绑定，否则Mesh会被胶囊体"拖着走"
		GetMesh()->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

		// 可选：给Mesh一个随机的微小旋转/冲量，让死亡更自然
		// GetMesh()->AddImpulse(FVector(FMath::RandRange(-100, 100), FMath::RandRange(-100, 100), 0));

		// TODO: 这里死了，会到处飘，还是播放死亡动画吧，受不了了，不会改
	}
}

void ADkCharacterHero::BindGASChangeDelegates()
{
	Super::BindGASChangeDelegates();
}

void ADkCharacterHero::OnLockingTargetStateChanged(bool InbIsLockingTarget)
{
	LerpCameraToLocalOffsetLocation(InbIsLockingTarget ? CameraLockTargetLocalOffset : FVector(0.f));
	LerpCameraBoomToTargetLength(InbIsLockingTarget ? CameraBoomLockingArmLength : CameraBoomDefaultArmLength);

	if (!InbIsLockingTarget)
	{
		Camera->SetRelativeRotation(FRotator(0.f));
	}
}

void ADkCharacterHero::LerpCameraToLocalOffsetLocation(const FVector& Goal)
{
	PendingCameraGoal = Goal;

	// 后续可以加个Tag，在1秒时间内不让玩家连续多次瞄准(已完成)
	// 这是为了防止连续取消，Set多个Timer导致镜头抖动
	// 如果已在插值中，只更新目标，不重启 Timer
	if (bIsCameraLerping)
	{
		return;
	}

	bIsCameraLerping = true;
	GetWorldTimerManager().ClearTimer(CameraLerpTimerHandle);
	CameraLerpTimerHandle = GetWorldTimerManager().SetTimerForNextTick(
		FTimerDelegate::CreateUObject(this, &ThisClass::TickCameraLocalOffsetLerp)
	);
}

void ADkCharacterHero::LerpCameraBoomToTargetLength(float InLength)
{
	PendingCameraBoomLength = InLength;

	if (bIsCameraBoomLerping)
	{
		return;
	}

	bIsCameraBoomLerping = true;
	GetWorldTimerManager().ClearTimer(CameraBoomLerpTimerHandle);
	CameraBoomLerpTimerHandle = GetWorldTimerManager().SetTimerForNextTick(
		FTimerDelegate::CreateUObject(this, &ThisClass::TickCameraBoomLengthLerp)
	);
}

void ADkCharacterHero::TickCameraLocalOffsetLerp()
{
	FVector CurrentLocalOffset = Camera->GetRelativeLocation();
	if (FVector::Dist(CurrentLocalOffset, PendingCameraGoal) < 1.f)
	{
		Camera->SetRelativeLocation(PendingCameraGoal);
		bIsCameraLerping = false; // 清除状态
		return;
	}

	float LerpAlpha = FMath::Clamp(GetWorld()->GetDeltaSeconds() * CameraLerpSpeed, 0.f, 1.f);
	FVector NewLocalOffset = FMath::Lerp(CurrentLocalOffset, PendingCameraGoal, LerpAlpha);
	Camera->SetRelativeLocation(NewLocalOffset);

	CameraLerpTimerHandle = GetWorldTimerManager().SetTimerForNextTick(
		FTimerDelegate::CreateUObject(this, &ThisClass::TickCameraLocalOffsetLerp)
	);
}

void ADkCharacterHero::TickCameraBoomLengthLerp()
{
	float CurrentArmLength = CameraBoom->TargetArmLength;
	if (FMath::IsNearlyEqual(CurrentArmLength, PendingCameraBoomLength))
	{
		CameraBoom->TargetArmLength = PendingCameraBoomLength;
		bIsCameraBoomLerping = false; // 清除状态
		return;
	}

	float LerpAlpha = FMath::Clamp(GetWorld()->GetDeltaSeconds() * CameraBoomLerpSpeed, 0.f, 1.f);
	float NewArmLength = FMath::Lerp(CurrentArmLength, PendingCameraBoomLength, LerpAlpha);
	CameraBoom->TargetArmLength = NewArmLength;
	CameraBoomLerpTimerHandle = GetWorldTimerManager().SetTimerForNextTick(
		FTimerDelegate::CreateUObject(this, &ThisClass::TickCameraBoomLengthLerp)
	);
}

void ADkCharacterHero::CameraFaceLockingTarget(float DeltaSeconds)
{
	AActor* LockTarget = AbilitySystemComponent->GetLockTarget();
	if (!IsValid(LockTarget)) return;

	float RotationSpeed = 10.0f;
	FVector LookEndPoint = LockTarget->GetActorLocation();
	FRotator CameraLookRotation = UKismetMathLibrary::FindLookAtRotation(Camera->GetComponentLocation(), LookEndPoint);
	FRotator CurCameraRotation = Camera->GetComponentRotation();
	FRotator NewRotation = FMath::RInterpTo(CurCameraRotation, CameraLookRotation, DeltaSeconds, RotationSpeed);
	Camera->SetWorldRotation(FRotator(FMath::Clamp(NewRotation.Pitch, -20.f, 20.f), NewRotation.Yaw, 0.f));
}

void ADkCharacterHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		for (const TPair<EAbilityInputID, UInputAction*>& InputActionPair :
		     AbilitySystemComponent->GetGameplayAbilityInputActions())
		{
			EnhancedInputComponent->BindAction(
				InputActionPair.Value, ETriggerEvent::Triggered, this,
				&ADkCharacterHero::HandleAbilityInput, InputActionPair.Key
			);
		}
	}
}

void ADkCharacterHero::HandleAbilityInput(const FInputActionValue& InputActionValue, EAbilityInputID InputID)
{
	bool bPressed = InputActionValue.Get<bool>();

	// if (bPressed && bIsLearnAbilityLeaderPressedDown)
	// {
	// 	UpgradeAbilityWithInputID(InputID);
	// 	return;
	// }

	if (bPressed)
	{
		GetAbilitySystemComponent()->AbilityLocalInputPressed((int32)InputID);
	}
	else
	{
		GetAbilitySystemComponent()->AbilityLocalInputReleased((int32)InputID);
	}
}
