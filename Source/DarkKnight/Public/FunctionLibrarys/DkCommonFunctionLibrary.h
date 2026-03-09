// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DkCommonFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkCommonFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	template <typename EnumType>
	static FString GetStringValueOfEnum(EnumType InEnumType)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();

		return StaticEnumOption->GetNameStringByIndex(static_cast<int64>(InEnumType));
	}

	template <typename EnumType>
	static FString GetDisplayValueOfEnum(EnumType InEnumType)
	{
		const UEnum* StaticEnumOption = StaticEnum<EnumType>();

		return StaticEnumOption->GetDisplayNameTextByIndex(static_cast<int64>(InEnumType));
	}

	UFUNCTION(BlueprintCallable, Category="Common Function Library")
	static bool IsStringPureNumber(const FString& Str);

	UFUNCTION(BlueprintCallable, Category="Common Function Library")
	static bool IsRightMouseClick(const FPointerEvent& MouseEvent);
	
	UFUNCTION(BlueprintCallable, Category="Common Function Library")
	static bool IsLeftMouseClick(const FPointerEvent& MouseEvent);
};
