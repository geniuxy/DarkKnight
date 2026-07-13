// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DkAnimInstanceMountBase.generated.h"

class ADkCharacterBase;
class ADkMountBase;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkAnimInstanceMountBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	//~Begin UAnimInstance Function
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	//~End UAnimInstance Function

	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	FORCEINLINE float GetSpeed() const { return Speed; }

	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	FORCEINLINE float GetLocomotionSpeed() const { return LocomotionSpeed; }

	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	FORCEINLINE float GetFwdSpeed() const { return FwdSpeed; }

	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	FORCEINLINE float GetRightSpeed() const { return RightSpeed; }

	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	FORCEINLINE bool IsMoving() const { return Speed != 0.f; }

	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	FORCEINLINE float GetYawSpeed() const { return YawSpeed; }

	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	FORCEINLINE float GetSmoothedYawSpeed() const { return SmoothedYawSpeed; }

	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	FORCEINLINE bool GetIsJumping() const { return bIsJumping; }

	UFUNCTION(BlueprintPure, meta=(BlueprintThreadSafe))
	FORCEINLINE bool GetIsOnGround() const { return !bIsJumping; }

protected:
	UPROPERTY()
	ADkMountBase* OwnerMountCharacter;

	UPROPERTY()
	UCharacterMovementComponent* OwnerMovementComp;

	UPROPERTY()
	ADkCharacterBase* OwnerInstigator;

	void CalculateMovementSpeed(float DeltaSeconds);
	float Speed;
	float LocomotionSpeed;
	float FwdSpeed;
	float RightSpeed;

	UPROPERTY(EditAnywhere, Category = "Animation")
	float TurnLeftOrRightLerpSpeed = 3.f;

	/* Yaw旋转速度 */
	FRotator BodyPrevRot;
	float YawSpeed;
	float SmoothedYawSpeed;

	UPROPERTY(EditAnywhere, Category = "Animation")
	float YawSpeedSmoothLerpSpeed = 1.f;
	UPROPERTY(EditAnywhere, Category = "Animation")
	float YawSpeedLerpToZeroSpeed = 30.f;
	/********/

	/* 跳跃相关 */
	bool bIsJumping;
	/********/

public:
	void SetOwnerInstigator(ADkCharacterBase* InOwnerInstigator) { OwnerInstigator = InOwnerInstigator; }
};
