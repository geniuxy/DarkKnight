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
	TObjectPtr<UDkInventoryLeafText> Text_ItemEntry_Main_0;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UDkInventoryLeafText> Text_ItemEntry_Main_1;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UCommonTextBlock> Text_ItemOptionalTitle;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UDkInventoryLeafText> Text_ItemEntry_Sub_0;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UDkInventoryLeafText> Text_ItemEntry_Sub_1;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UDkInventoryLeafText> Text_ItemEntry_Sub_2;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UDkInventoryLeafText> Text_ItemEntry_Sub_3;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UDkInventoryLeafText> Text_ItemDescription;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UDkInventoryLeafLabeledValue> Text_ItemSellValue;

	UPROPERTY(meta=(BindWidgetOptional))
	TObjectPtr<UDkInventoryLeafLabeledValue> Text_ItemRequiredLevel;
	//***** Bound Widgets *****//
};
