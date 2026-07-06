// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_CommitTask.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UAN_CommitTask : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference
	) override;

private:
	UPROPERTY(EditAnywhere, Category="AnimNotify Details")
	int MainTaskId;

	UPROPERTY(EditAnywhere, Category="AnimNotify Details")
	int SubTaskId;

	UPROPERTY(EditAnywhere, Category="AnimNotify Details")
	int CommitCount = 1;
};
