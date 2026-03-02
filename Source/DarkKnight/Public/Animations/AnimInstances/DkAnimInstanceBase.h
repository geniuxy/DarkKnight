// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DkAnimInstanceBase.generated.h"

enum class ELocomotionStyle : uint8;
class ADkCharacterBase;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	//~Begin UAnimInstance Function
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	//~End UAnimInstance Function

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<ADkCharacterBase> OwningCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Animation States|Locomotion")
	ELocomotionStyle LocomotionStyle;

	UPROPERTY(BlueprintReadOnly, Category = "Animation States|Locomotion")
	ELocomotionStyle LastLocomotionStyle;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Factors|Essential Data")
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Factors|Essential Data")
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Factors|Essential Data")
	float MaxSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Factors|Essential Data")
	FVector CurrentAcceleration;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Factors|Essential Data")
	FVector InputVector;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation State Factors|Essential Data")
	bool bHasMovementInput;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation State Factors|Essential Data")
	bool bJumping;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Factors|Essential Data")
	bool bIsFalling;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation State Factors|Essential Data")
	bool bIsMovingOnGround;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation State Factors|Rotation")
	FRotator Rotation;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Factors|Locomotion")
	float DeltaAngle;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Factors|Locomotion")
	float ActorYaw;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation State Factors|Locomotion")
	float LastActorYaw;
	
	UPROPERTY(BlueprintReadOnly, Category = "Animation State Factors|Locomotion")
	float DeltaActorYaw;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Animation State Factors|Locomotion")
	float YawAdjustmentFactor = 5.f;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Factors|Locomotion")
	float LeanAngle;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Factors|Locomotion")
	float StopFootSelection;
};
