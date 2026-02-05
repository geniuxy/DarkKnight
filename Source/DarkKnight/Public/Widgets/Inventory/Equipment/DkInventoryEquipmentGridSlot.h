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
 * 
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

	void UpdateEquipmentIcon(UDkInventoryItem* InItem);

	virtual void SetOccupiedBrush();
	virtual void SetEnabledBrush();
	virtual void SetDisabledBrush();
	virtual void SetGrayedOutBrush();

protected:
	//~Begin UUserWidget
	virtual void NativePreConstruct() override;
	//~End UUserWidget
	
	//***** Bound Widgets *****//
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> VertivalBox;

	UPROPERTY(meta = (BindWidgetOptional))
	TObjectPtr<UCommonTextBlock> Text_Title;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UDkInventoryEquipmentSlot> EquipmentSlot;
	//***** Bound Widgets *****//

	UPROPERTY(EditAnywhere, Category="Inventory")
	FSlateBrush NormalBrush;

	UPROPERTY(EditAnywhere, Category="Inventory")
	FSlateBrush EnabledBrush;

	UPROPERTY(EditAnywhere, Category="Inventory")
	FSlateBrush DisabledBrush;

	UPROPERTY(EditAnywhere, Category="Inventory")
	FSlateBrush GrayedOutBrush;

	UPROPERTY(EditAnywhere, Category="Inventory")
	FText SlotTitle;

	UPROPERTY(EditAnywhere, Category="Inventory")
	TSoftObjectPtr<UTexture2D> EquipmentBgIcon;

private:
	UPROPERTY(EditAnywhere, Category = "Inventory", meta = (Categories = "Dk.Item.Equipment"))
	FGameplayTag EquipmentTypeTag;

public:
	LIST_DATA_ACCESSOR(FGameplayTag, EquipmentTypeTag)
};
