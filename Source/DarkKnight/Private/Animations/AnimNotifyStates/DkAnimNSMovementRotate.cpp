// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotifyStates/DkAnimNSMovementRotate.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UDkAnimNSMovementRotate::NotifyTick(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float FrameDeltaTime,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	// 这一块是在Idle—>Walk/Run, Start的时候，期望调用的
	if (ACharacter* OwningCharacter = Cast<ACharacter>(MeshComp->GetOwner()))
	{
		FRotator CurrentRotation = OwningCharacter->GetActorRotation();
		// UKismetMathLibrary::Conv_VectorToRotator Roll = 0 ; Yaw 和 Pitch 不变
		FRotator TargetRotation =
			UKismetMathLibrary::Conv_VectorToRotator(OwningCharacter->GetCharacterMovement()->GetLastInputVector());

		FRotator InterpRotation =
			UKismetMathLibrary::RInterpTo(CurrentRotation, TargetRotation, FrameDeltaTime, InterpSpeed);

		OwningCharacter->SetActorRotation(InterpRotation);
	}
}
