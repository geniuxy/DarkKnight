// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GenericTeamAgentInterface.h"
#include "DarkKnight/DarkKnight.h"
#include "DkTypes/DkEnums.h"
#include "GameFramework/Character.h"
#include "DkCharacterBase.generated.h"

struct FRewardItemEntry;
class UDkActionComponent;
class UCharacterInfo;
class UDkAttributeSet;
class UDkAbilitySystemComponent;
class UMotionWarpingComponent;

UCLASS()
class DARKKNIGHT_API ADkCharacterBase : public ACharacter, public IAbilitySystemInterface,
                                        public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ADkCharacterBase();

	void ServerSideInit();
	void ClientSideInit();

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

	virtual void InitializeCharacterInfo();

	virtual void InitAbilityActorInfo();

	UPROPERTY(BlueprintReadOnly, Category= "Animation")
	ELocomotionStyle CurrentLocomotionStyle;

	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults")
	TObjectPtr<UCharacterInfo> CharacterInfo;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> BodyArmorMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> ArmArmorMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> BottomArmorMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> HelmetMesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMotionWarpingComponent> MotionWarping;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDkActionComponent> ActionComponent;

	/* 掉落物品 */
	UFUNCTION(Server, Reliable)
	void ServerSpawnRewardItemActor();

	UPROPERTY(EditAnywhere, Category="Inventory")
	float SpawnAngleMin = -135.f;

	UPROPERTY(EditAnywhere, Category="Inventory")
	float SpawnAngleMax = 135.f;

	UPROPERTY(EditAnywhere, Category="Inventory")
	float SpawnDistanceMin = 25.f;

	UPROPERTY(EditAnywhere, Category="Inventory")
	float SpawnDistanceMax = 50.f;

	UPROPERTY(EditAnywhere, Category="Inventory")
	float RelativeSpawnElevation = 70.f; // Spawn降低高度

	UPROPERTY(EditDefaultsOnly, Category="Reward")
	TArray<FRewardItemEntry> RewardItemIDList;
	/*********/

	/* 网络 */
	UPROPERTY(EditDefaultsOnly, Category="Net")
	bool bDebugNetworkInfo = false;

	void GetNetworkDebugInfo() const;
	/*********/

public:
	LIST_DATA_ACCESSOR(ELocomotionStyle, CurrentLocomotionStyle)

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

private:
	void BindGASChangeDelegates();

	void MoveSpeedUpdated(const FOnAttributeChangeData& Data);

	/**********************************************************************/
	/*                                Team                                */
	/**********************************************************************/
public:
	//~ Begin IGenericTeamAgentInterface Interface
	virtual void SetGenericTeamId(const FGenericTeamId& NewTeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
	//~ End IGenericTeamAgentInterface Interface
protected:
	FGenericTeamId TeamID;
};
