// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Composites/DkInventoryComposite.h"
#include "DkInventoryItemDescriptionMenu.generated.h"

class UDkInventoryLeafImage;
class USizeBox;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryItemDescriptionMenu : public UDkInventoryComposite
{
	GENERATED_BODY()

public:
	FVector2D GetBoxSize() const;

private:
	//***** Bound Widgets *****//
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;
	
	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UDkInventoryLeafImage> Image_ItemIcon;
	//***** Bound Widgets *****//
};
