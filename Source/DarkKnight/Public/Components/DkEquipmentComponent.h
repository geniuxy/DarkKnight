// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DkEquipmentComponent.generated.h"


struct FInventoryItemManifest;
struct FInventoryItemEquipmentFragment;
class ADkEquippedActorBase;
class UDkInventoryItem;
class ADkCharacterBase;
class UDkInventoryComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKKNIGHT_API UDkEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDkEquipmentComponent();

protected:
	virtual void BeginPlay() override;

private:
	TWeakObjectPtr<UDkInventoryComponent> InventoryComponent;
	TWeakObjectPtr<ADkCharacterBase> OwningCharacter;
	TWeakObjectPtr<APlayerController> OwningController;
	TWeakObjectPtr<USkeletalMeshComponent> OwningSkeletalMesh;

	UFUNCTION()
	void OnItemEquipped(UDkInventoryItem* EquippedItem);

	UFUNCTION()
	void OnItemUnEquipped(UDkInventoryItem* UnEquippedItem);

	ADkEquippedActorBase* SpawnEquippedActor(
		FInventoryItemEquipmentFragment* EquipmentFragment, USkeletalMeshComponent* AttachMesh
	);

	UPROPERTY()
	TArray<TObjectPtr<ADkEquippedActorBase>> EquippedActors;
};
