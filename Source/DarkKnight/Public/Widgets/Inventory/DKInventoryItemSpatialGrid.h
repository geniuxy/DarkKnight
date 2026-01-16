// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkInventoryItemGrid.h"
#include "DKInventoryItemSpatialGrid.generated.h"

class UCanvasPanel;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDKInventoryItemSpatialGrid : public UDkInventoryItemGrid
{
	GENERATED_BODY()

protected:
	//***** Bound Widgets *****//
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> GridCanvasPanel;
	//***** Bound Widgets *****//

	//~Begin UDkInventoryItemGrid
	virtual void ConstructGrid() override;
	//~End UDkInventoryItemGrid
};
