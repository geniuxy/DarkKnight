// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Inventory/DkInventoryItem.h"
#include "AddItemNoticeWidget.generated.h"

class UAddItemNoticeSlot;
class UVerticalBox;
class UDkInventoryComponent;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UAddItemNoticeWidget : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	//~Begin UUserWidget UFunction
	virtual void NativeOnInitialized() override;
	//~End UUserWidget UFunction

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UVerticalBox* AddItemNoticeBox;
	//***** Bound Widgets *****//

	UPROPERTY(EditDefaultsOnly, Category="Notice Sound")
	USoundBase* ItemGainedSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Notice Slot")
	TSubclassOf<UAddItemNoticeSlot> ItemNoticeSlotClass;

	UPROPERTY(EditDefaultsOnly, Category="Notice Slot")
	FText NoticeTips = FText().FromString(TEXT("已获得：{A} × {B}"));

	UPROPERTY(EditDefaultsOnly, Category="Notice Slot")
	float NoticeDuration = 3.f;

	UPROPERTY()
	UDkInventoryComponent* OwnerInventoryComp;
	
	UPROPERTY(Transient)
	TArray<FItemNoticeInfo> CachedItemNoticeList;
	
	UFUNCTION()
	void AddItemNotice(const FText& InItemName, int InItemCount);
	
	FTimerHandle ItemNoticeTimerHandle;
	void TryPlayItemNotice();
};
