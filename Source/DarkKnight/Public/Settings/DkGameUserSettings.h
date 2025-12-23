// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "DkGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	static UDkGameUserSettings* Get();

	//***** Gameplay Collection Tab ***** //

	UFUNCTION()
	FString GetCurrentGameDifficulty() const { return CurrentGameDifficulty; }

	UFUNCTION()
	void SetCurrentGameDifficulty(const FString& InNewDifficulty) { CurrentGameDifficulty = InNewDifficulty; }

	//***** Gameplay Collection Tab ***** //

private:
	UPROPERTY(Config)
	FString CurrentGameDifficulty;
};
