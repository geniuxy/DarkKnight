// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DeveloperSettings.h"
#include "DkUIDeveloperSettings.generated.h"

enum class EWidgetStackType : uint8;
class UDkWidgetActivatableBase;
/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "DarkKnight UI Settings"))
class DARKKNIGHT_API UDkUIDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = "Widget Reference", meta = (ForceInlineRow, Categories = "Dk.Widget"))
	TMap<FGameplayTag, TSoftClassPtr<UDkWidgetActivatableBase>> UIWidgetMap;

	UPROPERTY(Config, EditAnywhere, Category = "Widget Reference", meta = (ForceInlineRow, Categories = "Dk.WidgetStack"))
	TMap<EWidgetStackType, FGameplayTag> WidgetStackTagMap;

	UPROPERTY(Config, EditAnywhere, Category = "Image Reference", meta = (ForceInlineRow, Categories = "Dk.Image"))
	TMap<FGameplayTag, TSoftObjectPtr<UTexture2D>> ImageMap;
};
