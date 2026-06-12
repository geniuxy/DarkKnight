// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkCharacterBase.h"
#include "DkCharacterHero.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class UDkHeroAttributeSet;
class UPhysicsConstraintComponent;
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

	virtual void PawnClientRestart() override;

	void SwitchLocomotionStyle(ELocomotionStyle InStyle);

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode = 0) override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	/* Actor Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDkInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
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
	FORCEINLINE UDkInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	/**********************************************************************/
	/*                         Gameplay Ability                           */
	/**********************************************************************/
protected:
	UPROPERTY()
	UDkHeroAttributeSet* HeroAttributeSet;

	virtual void BindGASChangeDelegates() override;

private:
	virtual void OnLockingTargetStateChanged(bool InbIsLockingTarget) override;

	/**********************************************************************/
	/*                           Camera View                              */
	/**********************************************************************/
private:
	UPROPERTY(VisibleDefaultsOnly, Category="View")
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleDefaultsOnly, Category="View")
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category="View")
	FVector CameraLockTargetLocalOffset = FVector(0.f, 80.f, 20.f);

	UPROPERTY(EditDefaultsOnly, Category="View")
	float CameraBoomLockingArmLength = 500.f;
	
	UPROPERTY(EditDefaultsOnly, Category="View")
	float CameraBoomDefaultArmLength = 200.f;
	
	UPROPERTY(EditDefaultsOnly, Category="View")
	float CameraLerpSpeed = 20.f;

	UPROPERTY(EditDefaultsOnly, Category="View")
	float CameraBoomLerpSpeed = 20.f;

	FTimerHandle CameraLerpTimerHandle;
	FTimerHandle CameraBoomLerpTimerHandle;

	UPROPERTY()
	bool bIsCameraLerping = false;

	UPROPERTY()
	bool bIsCameraBoomLerping = false;

	UPROPERTY()
	FVector PendingCameraGoal;

	UPROPERTY()
	float PendingCameraBoomLength;
	
	void LerpCameraToLocalOffsetLocation(const FVector& Goal);
	void LerpCameraBoomToTargetLength(float InLength);
	void TickCameraLocalOffsetLerp();
	void TickCameraBoomLengthLerp();

	void CameraFaceLockingTarget(float DeltaSeconds);

	/**********************************************************************/
	/*                              Input                                 */
	/**********************************************************************/
public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
private:
	void HandleAbilityInput(const FInputActionValue& InputActionValue, EAbilityInputID InputID);
};
