// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DarkKnight/DarkKnight.h"
#include "DkTypes/DkEnums.h"
#include "DkActionComponent.generated.h"


enum class EOwnerType : uint8;
enum class EActionState : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKKNIGHT_API UDkActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDkActionComponent();

	void SetCurrentActionState(EActionState InActionState);

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
};
