// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DkAnimInstanceBase.generated.h"

class UDkAbilitySystemComponent;
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

	virtual void NativePostEvaluateAnimation() override;
	//~End UAnimInstance Function

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	bool ShouldDoUpperBodyBlend() const;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	TObjectPtr<ADkCharacterBase> OwningCharacter;

	UDkAbilitySystemComponent* GetOwnerASC();
	
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	UDkAbilitySystemComponent* OwnerASC;

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
	float DeltaAngle; // 目前朝向与输入方向的差值角度

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

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Factors|Locomotion")
	bool bCanTurnBack;

	bool bDoOnceAtTurnBack = true;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Factors|Locomotion")
	float MoveStartAngle; // 开始旋转时，朝向与输入方向的差值角度
	
	bool bDoOnceAtSetMoveStartAngle = true;

	UPROPERTY(BlueprintReadOnly, Category = "Animation State Factors|Essential Data")
	bool bInCombatState;
};
