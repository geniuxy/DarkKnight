// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Composites/DkInventoryComposite.h"
#include "DkInventoryItemDescriptionMenu.generated.h"

class UCommonTextBlock;
class UDkInventoryLeafLabeledValue;
class UDkInventoryLeafText;
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
	//~Begin UUserWidget
	virtual void NativeOnInitialized() override;
	//~End UUserWidget
	
	void ShowOptionalStatTitle() const;

	FVector2D GetBoxSize() const;

private:
	//***** Bound Widgets *****//
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox_Root;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UDkInventoryLeafImage> Image_ItemIcon;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UDkInventoryLeafText> Text_ItemName;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UDkInventoryLeafText> Text_ItemType;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UDkInventoryLeafLabeledValue> Text_ItemLabeledValue;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UCommonTextBlock> Text_ItemOptionalTitle;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UDkInventoryLeafLabeledValue> Text_ItemOptionalStat0;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UDkInventoryLeafLabeledValue> Text_ItemOptionalStat1;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UDkInventoryLeafLabeledValue> Text_ItemOptionalStat2;
	//***** Bound Widgets *****//
};
