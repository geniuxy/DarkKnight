// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "DataAssets/InputConfig.h"
#include "DkTypes/DkStructs.h"
#include "DkEnhancedInputComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKKNIGHT_API UDkEnhancedInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	template <class UserObject, typename CallbackFunc>
	void BindNativeInputAction(
		const UInputConfig* InInputConfig,
		const FGameplayTag& InInputTag,
		ETriggerEvent TriggerEvent,
		UserObject* ContextObject,
		CallbackFunc Func
	);

	template <class UserObject, typename CallbackFunc>
	void BindAbilityInputAction(
		const UInputConfig* InInputConfig,
		UserObject* ContextObject,
		CallbackFunc InputPressedFunc,
		CallbackFunc InputReleasedFunc
	);
};

template <class UserObject, typename CallbackFunc>
void UDkEnhancedInputComponent::BindNativeInputAction(
	const UInputConfig* InInputConfig,
	const FGameplayTag& InInputTag,
	ETriggerEvent TriggerEvent,
	UserObject* ContextObject,
	CallbackFunc Func)
{
	checkf(InInputConfig, TEXT("Input Data Asset is null, can not proceed with binding"));

	if (UInputAction* FoundAction = InInputConfig->FindNativeInputActionByTag(InInputTag))
	{
		BindAction(FoundAction, TriggerEvent, ContextObject, Func);
	}
}

template <class UserObject, typename CallbackFunc>
void UDkEnhancedInputComponent::BindAbilityInputAction(
	const UInputConfig* InInputConfig,
	UserObject* ContextObject,
	CallbackFunc InputPressedFunc,
	CallbackFunc InputReleasedFunc)
{
	checkf(InInputConfig, TEXT("Input Data Asset is null, can not proceed with binding"));

	for (const FDkInputActionConfig& AbilityInputActionConfig : InInputConfig->AbilityInputActions)
	{
		if (!AbilityInputActionConfig.IsValid()) continue;

		BindAction(
			AbilityInputActionConfig.InputAction,
			ETriggerEvent::Started,
			ContextObject,
			InputPressedFunc,
			AbilityInputActionConfig.InputTag
		);
		BindAction(
			AbilityInputActionConfig.InputAction,
			ETriggerEvent::Completed,
			ContextObject,
			InputReleasedFunc,
			AbilityInputActionConfig.InputTag
		);
	}
}
