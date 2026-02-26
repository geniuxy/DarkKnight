// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "DkAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DARKKNIGHT_API UDkAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UAttributeSet> OwningAttributeSet;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> AttributeSetInitialTable;
};
