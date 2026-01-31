// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkGameplayTags.h"
#include "DkInventoryLeaf.h"
#include "DkInventoryLeafText.generated.h"

class UCommonTextStyle;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryLeafText : public UDkInventoryLeaf
{
	GENERATED_BODY()

public:
	UDkInventoryLeafText()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Name;
	}
	
	void SetText(const FText& Text) const;

private:
	//***** Bound Widgets *****//
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonTextBlock> LeafText;
	//***** Bound Widgets *****//

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSubclassOf<UCommonTextStyle> TextStyle;
};
