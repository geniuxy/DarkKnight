// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkAnimInstanceBase.h"
#include "DkAnimInstanceNpc.generated.h"

class ADkCharacterNPC;
class UCharacterMovementComponent;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkAnimInstanceNpc : public UAnimInstance
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
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<ADkCharacterNPC> OwningCharacter;

	UPROPERTY()
	UCharacterMovementComponent* OwnerMovementComp;
	
	float Speed;

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
};
