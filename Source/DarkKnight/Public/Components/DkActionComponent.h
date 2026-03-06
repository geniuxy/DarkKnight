// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DarkKnight/DarkKnight.h"
#include "DkTypes/DkEnums.h"
#include "DkActionComponent.generated.h"


class UCharacterInfo;
class ADkCharacterBase;
enum class EOwnerType : uint8;
enum class EActionState : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKKNIGHT_API UDkActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDkActionComponent();

	void InitializeActorComponent(UCharacterInfo* InCharacterInfo);
	
	void SetCurrentActionState(EActionState InActionState);

protected:
	/* Actor Components */
	UPROPERTY()
	TObjectPtr<ADkCharacterBase> OwningCharacter;
	/*********/

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
};
