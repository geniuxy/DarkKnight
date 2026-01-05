// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonInputTypeEnum.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "Widgets/Options/DataObjects/DkUIListDataObjectBase.h"
#include "DkUIListDataObjectKeyRemap.generated.h"

class UEnhancedPlayerMappableKeyProfile;
class UEnhancedInputUserSettings;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkUIListDataObjectKeyRemap : public UDkUIListDataObjectBase
{
	GENERATED_BODY()

public:
	void InitKeyRemapData(
		UEnhancedInputUserSettings* InOwningInputUserSettings,
		UEnhancedPlayerMappableKeyProfile* InKeyProfile,
		ECommonInputType InDesiredInputKeyType,
		const FPlayerKeyMapping& InOwningPlayerKeyMapping
	);

	FSlateBrush GetIconFromCurrentKey() const;

private:
	FPlayerKeyMapping* GetOwningKeyMapping() const;
	
	UPROPERTY(Transient)
	UEnhancedInputUserSettings* CachedOwningInputUserSettings;

	UPROPERTY(Transient)
	UEnhancedPlayerMappableKeyProfile* CachedOwningKeyProfile;

	ECommonInputType CachedDesiredInputKeyType;

	FName CachedOwningMappingName;

	EPlayerMappableKeySlot CachedOwningMappableKeySlot;
};
