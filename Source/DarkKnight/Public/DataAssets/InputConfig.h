// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputConfig.generated.h"

struct FGameplayTag;
class UInputMappingContext;
class UInputAction;
struct FDkInputActionConfig;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UInputConfig : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty = "InputTag"))
	TArray<FDkInputActionConfig> NativeInputActions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(TitleProperty = "InputTag"))
	TArray<FDkInputActionConfig> AbilityInputActions;
	
	UInputAction* FindNativeInputActionByTag(const FGameplayTag& InInputTag) const;
};
