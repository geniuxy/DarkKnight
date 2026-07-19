// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimInstances/DkAnimInstanceNpc.h"

#include "Characters/NPC/DkCharacterNPC.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UDkAnimInstanceNpc::NativeInitializeAnimation()
{
	OwningCharacter = Cast<ADkCharacterNPC>(TryGetPawnOwner());
}

void UDkAnimInstanceNpc::NativeUpdateAnimation(float DeltaSeconds)
{
	if (OwningCharacter)
	{
		FVector Velocity = OwningCharacter->GetVelocity();
		Speed = Velocity.Length();

		FRotator BodyRot = OwningCharacter->GetActorRotation();
		FRotator BodyRotDelta = UKismetMathLibrary::NormalizedDeltaRotator(BodyRot, BodyPrevRot);
		BodyPrevRot = BodyRot;

		YawSpeed = BodyRotDelta.Yaw / DeltaSeconds;
		float YawLerpSpeed = YawSpeedSmoothLerpSpeed;
		if (YawSpeed == 0)
		{
			YawLerpSpeed = YawSpeedLerpToZeroSpeed;
		}
		SmoothedYawSpeed = UKismetMathLibrary::FInterpTo(SmoothedYawSpeed, YawSpeed, DeltaSeconds, YawLerpSpeed);
	}

	if (OwnerMovementComp)
	{
		bIsJumping = OwnerMovementComp->IsFalling();
	}
}

void UDkAnimInstanceNpc::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
}
