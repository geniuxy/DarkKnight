// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkGameplayTags.h"
#include "DkInventoryLeaf.h"
#include "DkInventoryLeafLabeledValue.generated.h"

class UCommonTextStyle;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryLeafLabeledValue : public UDkInventoryLeaf
{
	GENERATED_BODY()

public:
	UDkInventoryLeafLabeledValue()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_LabeledValue;
	}
	
	void SetTextLabel(const FText& Text, bool bCollapse) const;
	void SetTextValue(const FText& Text, bool bCollapse) const;

private:
	//***** Bound Widgets *****//
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> TextLabel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> TextValue;
	//***** Bound Widgets *****//

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UCommonTextStyle> TextLabelStyle;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UCommonTextStyle> TextValueStyle;
};
