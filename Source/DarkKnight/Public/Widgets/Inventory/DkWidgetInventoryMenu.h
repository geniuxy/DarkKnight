// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "DkWidgetInventoryMenu.generated.h"

class UWeightStatsGauge;
class UStatsGauge;
class UDkInventoryCategoryButton;
class UCommonListView;
class UDkInventoryComponent;
class UDkUICommonButtonImage;
class UDkItemComponent;
class UCommonLazyImage;
class UDkUICommonButtonBase;
class UDkInventoryItemGrid;
class UWidgetSwitcher;
class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DARKKNIGHT_API UDkWidgetInventoryMenu : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	void SetInventoryComponent(UDkInventoryComponent* InInventoryComp);

	void ClearGridItems();

protected:
	//~Begin UUserWidget Function
	virtual void NativeOnInitialized() override;
	//~End UUserWidget Function

	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> InventoryTitleTxt;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCommonListView* CategoryButtonListView;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UDkInventoryItemGrid> InventoryGrid;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UWeightStatsGauge> StatsGauge_Weight;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UStatsGauge> StatsGauge_Coins;
	//***** Bound Widgets *****//

	TWeakObjectPtr<UDkInventoryComponent> InventoryComponent;

	UPROPERTY()
	UDkInventoryCategoryButton* LastSelectedCategoryButton;

private:
	void InitInventoryCategoryButtons();
	void CategoryButtonPressed(UObject* SelectedUObject);
};
