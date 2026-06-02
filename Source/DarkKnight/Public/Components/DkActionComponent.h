// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkGameplayTags.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "DarkKnight/DarkKnight.h"
#include "DkTypes/DkEnums.h"
#include "DkActionComponent.generated.h"


class UCharacterInfo;
class ADkCharacterBase;
enum class EOwnerType : uint8;
enum class EActionState : uint8;

DECLARE_MULTICAST_DELEGATE_ThreeParams(
	FOnTriggerAction, FGameplayTag /* NewActionTag */, EActionPriority /* Priority */, bool /* bUseInputBuffer */)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKKNIGHT_API UDkActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDkActionComponent();

	void InitializeActorComponent(UCharacterInfo* InCharacterInfo);

	void SetCurrentActionState(EActionState InActionState);

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<ADkCharacterBase> OwnerCharacter;

private:
	bool bIsInCombat = false;

	EActionState CurrentActionState;
	EActionState LastActionState;
	EActionState LastActionStateWhenOnGround;

	EOwnerType OwnerType;

public:
	FORCEINLINE bool IsInCombatState() const { return bIsInCombat; }
	void SetIsInCombatState(bool IsInCombat) { bIsInCombat = IsInCombat; }

	FORCEINLINE EActionState GetCurrentActionState() const { return CurrentActionState; }

	LIST_DATA_ACCESSOR(EActionState, LastActionStateWhenOnGround)
	LIST_DATA_ACCESSOR(EOwnerType, OwnerType)

	/**********************************************************************/
	/*                        Action Input Buffer                         */
	/**********************************************************************/
public:
	FOnTriggerAction OnTriggerActionDelegate;

private:
	EActionPriority PreviousActionPriority;
	EActionPriority CurActionPriority;
	
	FGameplayTag CurActionTag = DkGameplayTags::Dk_Action_NoAction;

	void TriggerAction(FGameplayTag InCurrentActionTag, EActionPriority InCurrentActionPriority, bool bUseInputBuffer);
};
