// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define ECC_Item ECollisionChannel::ECC_GameTraceChannel1
#define ECC_Weapon ECollisionChannel::ECC_GameTraceChannel2
#define ECC_Horse ECollisionChannel::ECC_GameTraceChannel3
#define ECC_Fire ECollisionChannel::ECC_GameTraceChannel4
#define ECC_WeaponTrace ECollisionChannel::ECC_GameTraceChannel5

#define LIST_DATA_ACCESSOR(DataType, PropertyName) \
	FORCEINLINE DataType Get##PropertyName() const { return PropertyName; } \
	void Set##PropertyName(DataType In##PropertyName) { PropertyName = In##PropertyName; }
