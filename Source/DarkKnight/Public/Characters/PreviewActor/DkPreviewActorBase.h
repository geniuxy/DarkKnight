// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DkPreviewActorBase.generated.h"

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

	/* Init */
	FTimerHandle TimerForNextTick;
	void DelayedInitializeOwner();
	void DelayedInitialization();
	/********/
};
