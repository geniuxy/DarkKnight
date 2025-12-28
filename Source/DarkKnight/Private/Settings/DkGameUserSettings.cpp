// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/DkGameUserSettings.h"

UDkGameUserSettings::UDkGameUserSettings()
	: OverallVolume(1.f), MusicVolume(1.f), SoundFXVolume(1.f), bAllowBackgroundAudio(false)
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

void UDkGameUserSettings::SetMusicVolume(float InVolume)
{
	MusicVolume =  InVolume;
}

void UDkGameUserSettings::SetSoundFXVolume(float InVolume)
{
	SoundFXVolume = InVolume;
}

void UDkGameUserSettings::SetAllowBackgroundAudio(bool InbAllowBackgroundAudio)
{
	bAllowBackgroundAudio = InbAllowBackgroundAudio;
}
