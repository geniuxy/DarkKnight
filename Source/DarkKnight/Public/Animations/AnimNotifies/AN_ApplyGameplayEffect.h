// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_ApplyGameplayEffect.generated.h"

class UGameplayEffect;
/**
 * Tips:
 * 1. 可以用来在Montage的某个时刻，给人物加上个Tag(GE的GrantedTags需要加上对应的Tag)
 */
UCLASS()
class DARKKNIGHT_API UAN_ApplyGameplayEffect : public UAnimNotify
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
	TSubclassOf<UGameplayEffect> Effect;
};
