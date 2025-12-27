// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/DkGameUserSettings.h"

UDkGameUserSettings::UDkGameUserSettings()
	: OverallVolume(0.8f)
{
}

UDkGameUserSettings* UDkGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UDkGameUserSettings>(GEngine->GetGameUserSettings());
	}
	return nullptr;
}

void UDkGameUserSettings::SetOverallVolume(float InVolume)
{
	OverallVolume = InVolume;

	// 控制音量的实际逻辑写在这里
}
