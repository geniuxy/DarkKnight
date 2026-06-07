// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimInstances/DkAnimInstanceBase.h"

#include "AbilitySystemComponent.h"
#include "KismetAnimationLibrary.h"
#include "Characters/DkCharacterBase.h"
#include "Components/DkActionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UDkAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningCharacter = Cast<ADkCharacterBase>(GetOwningActor());

	// Animation Sequence的Enable Root Motion也会生效
	// 但不适合多人联网游戏时配置 在 Client 时，就会出现奇异的滚动
	SetRootMotionMode(ERootMotionMode::RootMotionFromEverything);

	LocomotionStyle = ELocomotionStyle::Walk;
	LastLocomotionStyle = ELocomotionStyle::Walk;
}

void UDkAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(OwningCharacter)) return;

	if (OwningCharacter->GetAbilitySystemComponent()->HasMatchingGameplayTag(DkGameplayTags::Dk_Stats_Sprint))
	{
		LocomotionStyle = ELocomotionStyle::Run;
	}
	else
	{
		LocomotionStyle = ELocomotionStyle::Walk;
	}
	Velocity = OwningCharacter->GetVelocity();
	Speed = Velocity.Length();
	MaxSpeed = OwningCharacter->GetCharacterMovement()->MaxWalkSpeed;
	CurrentAcceleration = OwningCharacter->GetCharacterMovement()->GetCurrentAcceleration();
	bJumping = OwningCharacter->bPressedJump;
	Rotation = OwningCharacter->GetActorRotation();

	bIsFalling = OwningCharacter->GetCharacterMovement()->IsFalling();
	bIsMovingOnGround = OwningCharacter->GetCharacterMovement()->IsMovingOnGround();

	InputVector = OwningCharacter->GetCharacterMovement()->GetLastInputVector().GetClampedToMaxSize(1.0f);
	if (InputVector.Length() > 0.f)
	{
		bHasMovementInput = true;
		DeltaAngle = UKismetAnimationLibrary::CalculateDirection(InputVector, Rotation);
	}
	else
	{
		bHasMovementInput = false;
	}

	LastActorYaw = ActorYaw;
	ActorYaw = Rotation.Yaw;
	DeltaActorYaw = ActorYaw - LastActorYaw;

	float YawAdjustmentRate = FMath::Clamp(DeltaActorYaw / (DeltaSeconds * YawAdjustmentFactor), -90.f, 90.f);

	LeanAngle = 0.f;
	if (LocomotionStyle == ELocomotionStyle::Walk)
	{
		LeanAngle = YawAdjustmentRate * 1.0f;
	}
	else if (LocomotionStyle == ELocomotionStyle::Run)
	{
		LeanAngle = YawAdjustmentRate * 0.5f;
	}

	if (bHasMovementInput)
	{
		StopFootSelection = GetCurveValue("StopFootSelection");
	}

	// 等价完成即将转身动作的判断
	if (bHasMovementInput)
	{
		if (bDoOnceAtSetMoveStartAngle)
		{
			MoveStartAngle = UKismetAnimationLibrary::CalculateDirection(InputVector, Rotation);
			bDoOnceAtSetMoveStartAngle= false;
		}

		// 企图往后转，且不在战斗状态中
		if ((DeltaAngle < -135.f || DeltaAngle > 135.f) && !bInCombatState)
		{
			if (bDoOnceAtTurnBack)
			{
				bCanTurnBack = true;
				bDoOnceAtTurnBack = false;
			}
		}
		else
		{
			bDoOnceAtTurnBack = true;
		}
	}
	else
	{
		bDoOnceAtSetMoveStartAngle = true;
	}

	UDkActionComponent* ActionComponent = OwningCharacter->FindComponentByClass<UDkActionComponent>();
	bInCombatState = ActionComponent->IsInCombatState();
}

void UDkAnimInstanceBase::NativePostEvaluateAnimation()
{
	Super::NativePostEvaluateAnimation();

	bCanTurnBack = false;
}
