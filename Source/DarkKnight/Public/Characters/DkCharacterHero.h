// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkCharacterBase.h"
#include "DkCharacterHero.generated.h"

class ADkPlayerStateBase;
class USpringArmComponent;
class UCameraComponent;
class UDkEquipmentComponent;
class UDkInventoryComponent;

UCLASS()
class DARKKNIGHT_API ADkCharacterHero : public ADkCharacterBase
{
	GENERATED_BODY()

public:
	ADkCharacterHero();
	
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	void SwitchLocomotionStyle(ELocomotionStyle InStyle);

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> CameraBoom;
	
	/* Actor Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Inventory")
	UDkInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Inventory")
	UDkEquipmentComponent* EquipmentComponent;
	/*********/

	UPROPERTY()
	TObjectPtr<ADkPlayerStateBase> OwningPlayerState;

	virtual void InitAbilityActorInfo() override;

	/* 坠亡相关 */
	FVector LastJumpStartPosition;

	UPROPERTY(EditAnywhere)
	float DeadlyFallHeight = 800.f;
	
	UFUNCTION()
	void HandleOnLanded(const FHitResult& Hit);

	void HandleFallDeath();
	/*********/

public:
	FORCEINLINE UDkInventoryComponent* GetInventoryComponent() const {return InventoryComponent;}
};
