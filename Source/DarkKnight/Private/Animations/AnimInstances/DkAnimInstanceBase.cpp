// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimInstances/DkAnimInstanceBase.h"

#include "KismetAnimationLibrary.h"
#include "Characters/DkCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"

void UDkAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningCharacter = Cast<ADkCharacterBase>(GetOwningActor());

	LocomotionStyle = ELocomotionStyle::Walk;
	LastLocomotionStyle = ELocomotionStyle::Walk;
}

void UDkAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(OwningCharacter)) return;

	LocomotionStyle = OwningCharacter->GetCurrentLocomotionStyle();
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
	DeltaAngle = ActorYaw - LastActorYaw;

	float YawAdjustmentRate = FMath::Clamp(DeltaAngle / (DeltaSeconds * YawAdjustmentFactor), -90.f, 90.f);

	LeanAngle = 0.f;
	if (LocomotionStyle == ELocomotionStyle::Walk)
	{
		LeanAngle = YawAdjustmentRate * 1.0f;
	}
	else if (LocomotionStyle == ELocomotionStyle::Run)
	{
		LeanAngle = YawAdjustmentRate * 0.5f;
	}

	if (!bHasMovementInput)
	{
		StopFootSelection = GetCurveValue("StopFootSelection");
	}
}
