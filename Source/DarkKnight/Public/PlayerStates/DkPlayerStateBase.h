// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/DkActionComponent.h"
#include "DarkKnight/DarkKnight.h"
#include "GameFramework/PlayerState.h"
#include "GAS/DkAbilitySystemComponent.h"
#include "GAS/DkAttributeSet.h"
#include "DkPlayerStateBase.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API ADkPlayerStateBase : public APlayerState
{
	GENERATED_BODY()

public:
	ADkPlayerStateBase();
	UDkAbilitySystemComponent* GetAbilitySystemComponent() const { return AbilitySystemComponent; }
	UDkAttributeSet* GetAttributeSet() const { return AttributeSet; }

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> AttributeSetInitialTable;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UDkAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UDkAttributeSet> AttributeSet;

	EActionState CurrentActionState; // 记录目前玩家的动作状态

public:
	LIST_DATA_ACCESSOR(EActionState, CurrentActionState)
};
