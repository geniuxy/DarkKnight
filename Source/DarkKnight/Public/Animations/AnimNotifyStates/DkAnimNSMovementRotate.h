// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "DkAnimNSMovementRotate.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkAnimNSMovementRotate : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category="Notify State Factor")
	float InterpSpeed = 3.f;

private:
	virtual void NotifyTick(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		float FrameDeltaTime,
		const FAnimNotifyEventReference& EventReference
	) override;

	
};
