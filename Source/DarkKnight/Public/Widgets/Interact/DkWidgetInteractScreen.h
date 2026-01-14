// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "DkWidgetInteractScreen.generated.h"

class UDkWidgetInfoMessage;
class UDkWidgetPickUpBox;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DARKKNIGHT_API UDkWidgetInteractScreen : public UDkWidgetActivatableBase
{
	GENERATED_BODY()

public:
	void ShowPickUpMessage(const FString& Message);
	void HidePickUpMessage();

protected:
	//~Begin UUserWidget
	virtual void NativeOnInitialized() override;
	//~End UUserWidget

private:
	UFUNCTION()
	void ShowNoRoomInInventory();
	
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UDkWidgetPickUpBox* WBP_PickUpBox;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UDkWidgetInfoMessage* WBP_InfoMessage;
	//***** Bound Widgets *****//
};
