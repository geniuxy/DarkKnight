// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DkCharacterHero.h"
#include "DkCharacterManny.generated.h"

UCLASS()
class DARKKNIGHT_API ADkCharacterManny : public ADkCharacterHero
{
	GENERATED_BODY()

public:
	ADkCharacterManny();
	void InitPrimaryWeaponComponent();
	void InitSecondaryWeaponComponent();

protected:
	/* Actor Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* PrimaryWeaponStorePoint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* PrimaryWeaponSheathRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPhysicsConstraintComponent* PrimaryPhysicsConstraint; // 为了制作武器摇曳的效果
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* PrimaryWeaponSheathTarget;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* SecondaryWeaponStorePoint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* SecondaryWeaponSheathRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UPhysicsConstraintComponent* SecondaryPhysicsConstraint; // 为了制作武器摇曳的效果
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* SecondaryWeaponSheathTarget;
	/*********/
};
