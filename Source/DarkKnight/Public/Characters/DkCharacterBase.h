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

class UWidgetComponent;
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

	bool IsLocallyControlledByPlayer();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

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

	UPROPERTY(EditAnywhere, Category="Reward")
	TArray<FRewardItemEntry> RewardItemIdList;
	/*********/

	/* 网络 */
	UPROPERTY(EditDefaultsOnly, Category="Net")
	bool bDebugNetworkInfo = false;

	void GetNetworkDebugInfo() const;
	/*********/

public:
	LIST_DATA_ACCESSOR(ELocomotionStyle, CurrentLocomotionStyle)

	/**********************************************************************/
	/*                            Collision                               */
	/**********************************************************************/

public:
	void DisableCollisionToPawn();

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

	virtual void BindGASChangeDelegates();
	
	bool bIsLockingTarget = false;

private:
	void LockingTargetTagUpdated(FGameplayTag Tag, int NewCount);
	virtual void OnLockingTargetStateChanged(bool InbIsLockingTarget);
	void FaceLockTarget(float DeltaSeconds);

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
	UPROPERTY(EditInstanceOnly)
	FGenericTeamId TeamID;

	/**********************************************************************/
	/*                         UI - OverHeadStats                         */
	/**********************************************************************/
public:
	void ConfigureOverHeadStatsWidget();

private:
	UPROPERTY(VisibleDefaultsOnly, Category="UI")
	UWidgetComponent* OverHeadWidgetComponent;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	float HeadStatsGaugeVisibilityCheckUpdateGap = 1.f;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	float HeadStatsGaugeVisibilityRangeSquared = 1000000.f; // 头顶血条可视距离的平方

	FTimerHandle HeadStatsGaugeVisibilityUpdateTimerHandle;

	void UpdateHeadGaugeVisibility();

	void SetStatusGaugeEnabled(bool bIsEnabled);
};
