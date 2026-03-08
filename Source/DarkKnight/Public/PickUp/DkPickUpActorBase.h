// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DkPickUpActorBase.generated.h"

struct FDkItemInfo;
class UDkHighlightableSkeletalMesh;
class UDkItemComponent;

UCLASS()
class DARKKNIGHT_API ADkPickUpActorBase : public AActor
{
	GENERATED_BODY()

public:
	ADkPickUpActorBase();

	virtual void SetPickUpItemInfo(const FDkItemInfo* PickUpItemInfo, int32 InItemStack);

protected:
	/* Actor Components */
	UPROPERTY(VisibleAnywhere)
	UDkItemComponent* ItemComponent;
	/*********/
};
