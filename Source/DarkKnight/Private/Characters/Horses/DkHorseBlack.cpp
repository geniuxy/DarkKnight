// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Horses/DkHorseBlack.h"

ADkHorseBlack::ADkHorseBlack()
{
	MountPoint_FrontR->SetRelativeLocation(FVector(-62.613391f, 66.108777f, 82.f));
	MountPoint_FrontL->SetRelativeLocation(FVector(62.613391f, 66.108777f, 82.f));
	MountPoint_R->SetRelativeLocation(FVector(-62.39968f, 2.580163f, 99.763305f));
	MountPoint_L->SetRelativeLocation(FVector(62.39968f, -6.424226f, 97.547409f));
	MountPoint_B->SetRelativeLocation(FVector(0.f, -95.0f, 92.0f));
	CameraOriginalLocation->SetRelativeLocation(FVector(0.f, 69.0895f, 126.118647f));
}

