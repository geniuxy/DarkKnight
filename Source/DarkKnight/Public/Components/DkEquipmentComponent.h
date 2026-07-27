// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "DkEquipmentComponent.generated.h"


class UDkPlayerInventoryComp;
class UGameplayEffect;
class UDkAbilitySystemComponent;
struct FInventoryItemManifest;
struct FInventoryItemFragment_Equipment;
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
	TWeakObjectPtr<UDkPlayerInventoryComp> InventoryComponent;
	TWeakObjectPtr<ADkCharacterBase> OwningCharacter;
	TWeakObjectPtr<APlayerController> OwningController;
	TWeakObjectPtr<USkeletalMeshComponent> OwningSkeletalMesh;
	TWeakObjectPtr<UDkAbilitySystemComponent> OwnerASC;

	/* Init */
	void InitPlayerCharacter();

	UFUNCTION()
	void OnControllerChanged(APawn* Pawn, AController* OldController, AController* NewController);

	void InitInventoryComponent();
	void InitOwnerASC();
	/********/

	/* 装备Item */
	UFUNCTION()
	void OnItemEquipped(UDkInventoryItem* EquippedItem); // 仅在bIsPreview=true || OwningController->HasAuthority()时执行

	UFUNCTION()
	void OnItemUnEquipped(UDkInventoryItem* UnEquippedItem); // 仅在bIsPreview=true || OwningController->HasAuthority()时执行

	ADkEquippedActorBase* SpawnEquippedActor(
		FInventoryItemFragment_Equipment* EquipmentFragment, USkeletalMeshComponent* AttachMesh
	);

	ADkEquippedActorBase* FindEquippedActor(const FGameplayTag& EquippedActorTag);

	ADkEquippedActorBase* FindTargetTypeEquippedActor(const FGameplayTag& TargetTypeTag);

	UPROPERTY()
	TArray<TObjectPtr<ADkEquippedActorBase>> EquippedActors;

	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TSubclassOf<UGameplayEffect> EquippedEffect;

	void RemoveEquippedActor(FInventoryItemFragment_Equipment* EquipmentFragment);
	/********/

	/**********************************************************************/
	/*                       Preview Actor Setting                        */
	/**********************************************************************/
public:
	void SetOwningSkeletalMesh(USkeletalMeshComponent* InOwningMesh) { OwningSkeletalMesh = InOwningMesh; }
	void SetIsPreview(bool bInIsPreview);
	void InitializeOwner(APlayerController* PlayerController);

private:
	bool bIsPreview = false;

	/**********************************************************************/
	/*                            Draw Weapon                             */
	/**********************************************************************/
public:
	void DrawWeapon(FGameplayTag InWeaponTypeTag, FName InSocketName = NAME_None);
	void SheatheWeapon(FGameplayTag InWeaponTypeTag, FName InSocketName = NAME_None);
};
