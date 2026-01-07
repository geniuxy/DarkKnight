// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "DkLoadingScreenSettings.generated.h"

/**
 * 
 */
UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "DarkKnight Loading Screen Settings"))
class DARKKNIGHT_API UDkLoadingScreenSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	TSubclassOf<UUserWidget> GetLoadingScreenWidgetClassChecked() const;

	UPROPERTY(Config, EditAnywhere, Category="Loading Screen Settings")
	TSoftClassPtr<UUserWidget> SoftLoadingScreenWidgetClass;

	UPROPERTY(Config, EditAnywhere, Category="Loading Screen Settings")
	float HoldingLoadingScreenExtraSeconds = 3.f;
	
	UPROPERTY(Config, EditAnywhere, Category="Loading Screen Settings")
	bool bShouldShowLoadingScreenInEditor = false;
};
