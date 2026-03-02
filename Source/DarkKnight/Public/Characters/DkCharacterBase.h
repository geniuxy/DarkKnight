// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DarkKnight/DarkKnight.h"
#include "GameFramework/Character.h"
#include "DkTypes/DkEnums.h"
#include "DkCharacterBase.generated.h"

class UDkActionComponent;
class UCharacterInfo;
class UDkAttributeSet;
class UDkAbilitySystemComponent;
class UMotionWarpingComponent;

UCLASS()
class DARKKNIGHT_API ADkCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ADkCharacterBase();

	void SwitchLocomotionStyle(ELocomotionStyle InStyle);

protected:
	virtual void BeginPlay() override;

	/* GAS */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "GAS")
	TObjectPtr<UDkAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "GAS")
	TObjectPtr<UDkAttributeSet> AttributeSet;
	/*********/

	virtual void InitializeCharacterInfo();

	virtual void InitAbilityActorInfo();

	UPROPERTY(BlueprintReadOnly, Category= "Animation")
	ELocomotionStyle CurrentLocomotionStyle;

	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults")
	TObjectPtr<UCharacterInfo> CharacterInfo;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> BodyArmorMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> ArmArmorMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> BottomArmorMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> HelmetMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMotionWarpingComponent> MotionWarping;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDkActionComponent> ActionComponent;

public:
	LIST_DATA_ACCESSOR(ELocomotionStyle, CurrentLocomotionStyle)
};
