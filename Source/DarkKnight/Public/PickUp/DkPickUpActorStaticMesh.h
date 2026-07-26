// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkPickUpActorBase.h"
#include "DkPickUpActorStaticMesh.generated.h"

class UDkHighlightableStaticMesh;

UCLASS()
class DARKKNIGHT_API ADkPickUpActorStaticMesh : public ADkPickUpActorBase
{
	GENERATED_BODY()

public:
	ADkPickUpActorStaticMesh();

	virtual void SetPickUpItemInfo(const FDkItemInfo& PickUpItemInfo, int32 InItemStack) override;

	void SetItemStaticMesh(const FDkItemInfo& PickUpItemInfo);
	
protected:
	/* Actor Components */
	UPROPERTY(VisibleAnywhere)
	UDkHighlightableStaticMesh* PickUpItemStaticMesh;
	/*********/
};
