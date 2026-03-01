// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/InputConfig.h"

#include "DkTypes/DkStructs.h"

UInputAction* UInputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	for (FDkInputActionConfig InputActionConfig : NativeInputActions)
	{
		if (InputActionConfig.InputTag == InInputTag && InputActionConfig.InputAction)
		{
			return InputActionConfig.InputAction;
		}
	}
	return nullptr;
}
