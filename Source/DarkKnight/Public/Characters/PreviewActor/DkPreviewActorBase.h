// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DkPreviewActorBase.generated.h"

class USpringArmComponent;
class UPhysicsConstraintComponent;
class UDkEquipmentComponent;

UCLASS()
class DARKKNIGHT_API ADkPreviewActorBase : public AActor
{
	GENERATED_BODY()

public:
	ADkPreviewActorBase();

	USkeletalMeshComponent* GetMesh() const { return Mesh; }

protected:
	virtual void BeginPlay() override;

private:
	// PlayerController对应的角色Mesh
	TWeakObjectPtr<USkeletalMeshComponent> SourceMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDkEquipmentComponent> EquipmentComponent;

	// PreviewActor对应的Mesh
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> BodyArmorMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> ArmArmorMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> BottomArmorMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> HelmetMesh;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* PrimaryWeaponStorePoint;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PrimaryWeaponSheathRoot;

	UPROPERTY(VisibleAnywhere)
	UPhysicsConstraintComponent* PrimaryPhysicsConstraint; // 为了制作武器摇曳的效果

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PrimaryWeaponSheathTarget;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SecondaryWeaponStorePoint;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SecondaryWeaponSheathRoot;

	UPROPERTY(VisibleAnywhere)
	UPhysicsConstraintComponent* SecondaryPhysicsConstraint; // 为了制作武器摇曳的效果

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* SecondaryWeaponSheathTarget;

	UPROPERTY(VisibleDefaultsOnly, Category="Render Actor")
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, Category="Render Actor")
	USceneCaptureComponent2D* CaptureComponent;

	/* Init */
	FTimerHandle TimerForNextTick;
	void DelayedInitializeOwner();
	void DelayedInitialization();

	void InitPrimaryWeaponComponent();
	void InitSecondaryWeaponComponent();
	/********/

	/**********************************************************************/
	/*                            Render Actor                            */
	/**********************************************************************/
public:
	void SetRenderTarget(UTextureRenderTarget2D* RenderTarget);
	void UpdateRender();
	
public:
	FORCEINLINE USceneCaptureComponent2D* GetCaptureComponent() const { return CaptureComponent; }
};
