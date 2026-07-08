// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/Interface.h"
#include "TaskTrackingInterface.generated.h"

struct FGameplayTag;
UINTERFACE()
class UTaskTrackingInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DARKKNIGHT_API ITaskTrackingInterface
{
	GENERATED_BODY()

public:
	virtual FGameplayTag GetTaskTrackingTag() const { return FGameplayTag(); }
	virtual void SetTaskTrackingTag(FGameplayTag InTag) { }
};
