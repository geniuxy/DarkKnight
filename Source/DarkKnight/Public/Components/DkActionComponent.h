// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DarkKnight/DarkKnight.h"
#include "DkActionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKKNIGHT_API UDkActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDkActionComponent();

private:
	bool bIsInCombat = false;

public:
	FORCEINLINE bool IsInCombatState() const { return bIsInCombat; }
	void SetIsInCombatState(bool IsInCombat) { bIsInCombat = IsInCombat; }
};
