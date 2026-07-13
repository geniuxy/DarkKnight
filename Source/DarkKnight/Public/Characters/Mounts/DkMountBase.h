// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GenericTeamAgentInterface.h"
#include "DkTypes/DkEnums.h"
#include "GameFramework/Character.h"
#include "DkMountBase.generated.h"

enum class EMountMoveType : uint8;
class ADkCharacterBase;
enum class EAbilityInputID : uint8;
struct FInputActionValue;
class UDkAttributeSet;
class UDkAbilitySystemComponent;
class UCameraComponent;
class USpringArmComponent;
class USphereComponent;
class UWidgetComponent;
class UBoxComponent;

UCLASS()
class DARKKNIGHT_API ADkMountBase : public ACharacter, public IGenericTeamAgentInterface, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ADkMountBase();

	virtual void PawnClientRestart() override;

protected:
	virtual void Tick(float DeltaSeconds) override;

	/* Actor Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* PushCrowdBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* InteractionWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USphereComponent* MountSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* Mane; // 鬃毛

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* Tail; // 尾巴

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* Saddle; // 马鞍

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USkeletalMeshComponent* Reins; // 缰绳

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* MountPoint_FrontR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* MountPoint_FrontL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* MountPoint_R;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* MountPoint_L;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* MountPoint_B;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* CameraOriginalLocation;
	/*********/

	bool IsLocallyControlledByPlayer();

	/**********************************************************************/
	/*                            Instigator                              */
	/**********************************************************************/
public:
	ADkCharacterBase* GetOwnerInstigator();

	/**********************************************************************/
	/*                         Gameplay Ability                           */
	/**********************************************************************/
public:
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(VisibleAnywhere, Category= "GAS")
	TObjectPtr<UDkAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(VisibleAnywhere, Category= "GAS")
	TObjectPtr<UDkAttributeSet> AttributeSet;

	/**********************************************************************/
	/*                              Input                                 */
	/**********************************************************************/
public:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	void HandleAbilityInput(const FInputActionValue& InputActionValue, EAbilityInputID InputID);

	/**********************************************************************/
	/*                              Camera                                */
	/**********************************************************************/
protected:
	void UpdateCameraRotation(float DeltaSeconds);
	void StartCameraFaceForward(float DeltaSeconds);
	void HandleCameraFaceForward();

	float DeltaRotator = 0.f;

	bool bStartCameraLocateForward = false;
	bool bTempStopCameraLocateForward = false;

	FTimerHandle StartCameraFaceForwardHandle;

public:
	void SetTempStopCameraLocateForward(bool bStop) { bTempStopCameraLocateForward = bStop; }

	/**********************************************************************/
	/*                         Desired Movement                           */
	/**********************************************************************/
public:
	FVector GetDesiredMovement();

private:
	float MovementInputX = 0.f;
	float MovementInputY = 0.f;

	float LocomotionExpectedSpeed = 0.f;
	float LocomotionInterpSpeed = 300.f;
	float LocomotionActualSpeed = 0.f;
	EMountMoveType MoveType = EMountMoveType::Idle;

	FVector AITargetLocation = FVector();

public:
	void SetMovementInputX(float InActionX) { MovementInputX = InActionX; }
	void SetMovementInputY(float InActionY) { MovementInputY = InActionY; }
	void SetLocomotionExpectedSpeed(float InValue) { LocomotionExpectedSpeed = InValue; }
	void SetLocomotionInterpSpeed(float InValue) { LocomotionActualSpeed = InValue; }
	FORCEINLINE float GetLocomotionActualSpeed() const { return LocomotionActualSpeed; }
	FORCEINLINE EMountMoveType GetMoveType() const { return MoveType; }
	void SetMoveType(EMountMoveType InMoveType) { MoveType = InMoveType; }

	/**********************************************************************/
	/*                                Team                                */
	/**********************************************************************/
public:
	//~ Begin IGenericTeamAgentInterface Interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	//~ End IGenericTeamAgentInterface Interface

protected:
	UPROPERTY(EditInstanceOnly)
	FGenericTeamId TeamID;
};
