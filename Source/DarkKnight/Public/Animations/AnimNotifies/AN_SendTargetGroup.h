// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GenericTeamAgentInterface.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_SendTargetGroup.generated.h"

struct FGameplayTagContainer;
/**
 * 动画某一帧，把Socket相连有重叠的目标都通过Event传给Owner
 */
UCLASS()
class DARKKNIGHT_API UAN_SendTargetGroup : public UAnimNotify
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
	FGameplayTagContainer TriggerGameplayCueTags;

	UPROPERTY(EditAnywhere, Category="AnimNotify Details")
	TEnumAsByte<ETeamAttitude::Type> TargetTeam = ETeamAttitude::Hostile;

	UPROPERTY(EditAnywhere, Category="AnimNotify Details")
	float SphereSweepRadius = 60.f;

	UPROPERTY(EditAnywhere, Category="AnimNotify Details")
	bool bDrawDebug = false;

	UPROPERTY(EditAnywhere, Category="AnimNotify Details")
	bool bIgnoreOwner = true;

	UPROPERTY(EditAnywhere, Category="AnimNotify Details")
	TArray<FName> TargetSocketNames;

	UPROPERTY(EditAnywhere, Category="AnimNotify Details")
	FGameplayTag EventTag;

	void SendLocalGameplayCue(const FHitResult& HitResult) const;
};
