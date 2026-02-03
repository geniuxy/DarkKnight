// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DkInventoryEquipmentGridSlot.generated.h"

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
	FText SlotTitle;
};
