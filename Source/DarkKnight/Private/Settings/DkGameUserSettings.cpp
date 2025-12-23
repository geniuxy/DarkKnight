// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/DkGameUserSettings.h"

UDkGameUserSettings* UDkGameUserSettings::Get()
{
	if (GEngine)
	{
		return CastChecked<UDkGameUserSettings>(GEngine->GetGameUserSettings());
	}
	return nullptr;
}
