// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "DarkKnight/DarkKnight.h"
#include "DkInventoryEquipmentGridSlot.generated.h"

class UDkInventoryItem;
class UCommonTextBlock;
class UVerticalBox;
class UDkInventoryEquipmentSlot;
class USizeBox;
/**
 * 装备物品网格(包括网格和装备类标题)
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryEquipmentGridSlot : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	FVector2D GetTotalSlotSize() const;
	FVector2D GetSlotSize() const;
	UDkInventoryEquipmentSlot* GetEquipmentSlot() const;

	UDkInventoryItem* GetInventoryItem() const;

	void UpdateEquipmentInfo(UDkInventoryItem* InItem);

	void SetUnoccupiedBrush();
	void SetEnabledBrush();
	void SetDisabledBrush();
	void SetGrayedOutBrush();
	void SetDefaultBackGroundIcon();

protected:
	//~Begin UUserWidget
	virtual void NativePreConstruct() override;
	//~End UUserWidget
	
	//***** Bound Widgets *****//
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VerticalBox;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock> Text_Title;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDkInventoryEquipmentSlot> EquipmentSlot;
	//***** Bound Widgets *****//

	UPROPERTY(EditAnywhere, Category="Inventory")
	FText SlotTitle;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSoftObjectPtr<UTexture2D> EquipmentBgIcon;

private:
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Categories = "Dk.Item"))
	FGameplayTag EquipmentTypeTag;

public:
	LIST_DATA_ACCESSOR(FGameplayTag, EquipmentTypeTag)
};
