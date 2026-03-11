// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkPickUpActorBase.h"
#include "DkPickUpActorSkeletalMesh.generated.h"

UCLASS()
class DARKKNIGHT_API ADkPickUpActorSkeletalMesh : public ADkPickUpActorBase
{
	GENERATED_BODY()

public:
	ADkPickUpActorSkeletalMesh();

	virtual void SetPickUpItemInfo(const FDkItemInfo* PickUpItemInfo, int32 InItemStack) override;

	void SetItemSkeletalMesh(const FDkItemInfo* PickUpItemInfo);

protected:
	/* Actor Components */
	UPROPERTY(VisibleAnywhere)
	UDkHighlightableSkeletalMesh* PickUpItemSkeletalMesh;
	/*********/
};
