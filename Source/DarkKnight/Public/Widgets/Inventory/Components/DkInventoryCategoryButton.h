// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Blueprint/IUserObjectListEntry.h"
#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DkInventoryCategoryButton.generated.h"

class UCommonListView;
class UInventoryItemCategoryData;
class UCommonLazyImage;
class UDkUICommonButtonImage;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryCategoryButton : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	//~Begin IUserObjectListEntry Interface
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	//~End IUserObjectListEntry Interface

	void ToggleHighlightState(bool bShouldHighlight);

	void HandleCategoryButtonClicked();

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UDkUICommonButtonImage> CategoryButton;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCommonLazyImage> SelectedUnderline;

	UPROPERTY()
	UInventoryItemCategoryData* CategoryData;

	UPROPERTY()
	UCommonListView* OwnerListView;
};
