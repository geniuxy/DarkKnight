// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DkHorseBase.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USphereComponent;
class UWidgetComponent;
class UBoxComponent;

UCLASS()
class DARKKNIGHT_API ADkHorseBase : public ACharacter
{
	GENERATED_BODY()

public:
	ADkHorseBase();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
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
};
