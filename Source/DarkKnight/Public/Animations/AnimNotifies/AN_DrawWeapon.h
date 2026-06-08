// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_DrawWeapon.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UAN_DrawWeapon : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference
	) override;

private:
	UPROPERTY(EditAnywhere, Category="AnimNotify Details", meta = (Categories = "Dk.Item.Equipment.Weapons"))
	FGameplayTag WeaponTypeTag;

	UPROPERTY(EditAnywhere, Category="AnimNotify Details")
	FName SocketName;

	UPROPERTY(EditAnywhere, Category="AnimNotify Details")
	bool bDrawWeapon = true;
};
