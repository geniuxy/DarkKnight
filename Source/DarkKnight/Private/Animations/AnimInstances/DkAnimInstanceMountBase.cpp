// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimInstances/DkAnimInstanceMountBase.h"

#include "Characters/DkCharacterBase.h"
#include "Characters/Mounts/DkMountBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UDkAnimInstanceMountBase::NativeInitializeAnimation()
{
	OwnerMountCharacter = Cast<ADkMountBase>(TryGetPawnOwner());
	if (OwnerMountCharacter)
	{
		OwnerMovementComp = OwnerMountCharacter->GetCharacterMovement();
	}
}

void UDkAnimInstanceMountBase::NativeUpdateAnimation(float DeltaSeconds)
{
	if (OwnerMountCharacter)
	{
		FVector Velocity = OwnerMountCharacter->GetVelocity();
		Speed = Velocity.Length();

		FRotator BodyRot = OwnerMountCharacter->GetActorRotation();
		FRotator BodyRotDelta = UKismetMathLibrary::NormalizedDeltaRotator(BodyRot, BodyPrevRot);
		BodyPrevRot = BodyRot;

		YawSpeed = BodyRotDelta.Yaw / DeltaSeconds;
		float YawLerpSpeed = YawSpeedSmoothLerpSpeed;
		if (YawSpeed == 0)
		{
			YawLerpSpeed = YawSpeedLerpToZeroSpeed;
		}
		SmoothedYawSpeed = UKismetMathLibrary::FInterpTo(SmoothedYawSpeed, YawSpeed, DeltaSeconds, YawLerpSpeed);

		CalculateMovementSpeed(DeltaSeconds);
	}

	if (OwnerMovementComp)
	{
		bIsJumping = OwnerMovementComp->IsFalling();
	}

	if (OwnerInstigator)
	{
	}
}

void UDkAnimInstanceMountBase::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
}


void UDkAnimInstanceMountBase::CalculateMovementSpeed(float DeltaSeconds)
{
	// FRotator ControlRot = OwnerMountCharacter->GetBaseAimRotation();
	// FwdSpeed = Velocity.Dot(ControlRot.Vector()); // .Vector()会将转向变成单位向量
	// RightSpeed = -Velocity.Dot(ControlRot.Vector().Cross(FVector::UpVector));

	LocomotionSpeed = OwnerMountCharacter->GetLocomotionActualSpeed();

	FwdSpeed = OwnerMountCharacter->GetDesiredMovement().Dot(OwnerMountCharacter->GetActorForwardVector());
	if (FwdSpeed < 0.f) // 倒退的情况
	{
		float TargetRightSpeed = FMath::Clamp(
			OwnerMountCharacter->GetDesiredMovement().Dot(OwnerMountCharacter->GetActorRightVector()) * 10.f,
			-1.f,
			1.f
		);

		RightSpeed = UKismetMathLibrary::FInterpTo(
			RightSpeed, TargetRightSpeed, DeltaSeconds, TurnLeftOrRightLerpSpeed * 10.f
		);
	}
	else
	{
		float TargetRightSpeed =
			OwnerMountCharacter->GetDesiredMovement().Dot(OwnerMountCharacter->GetActorRightVector());

		RightSpeed = UKismetMathLibrary::FInterpTo(
			RightSpeed, TargetRightSpeed, DeltaSeconds, TurnLeftOrRightLerpSpeed
		);
	}
}
