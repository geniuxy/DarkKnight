// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "DkWidgetPickUpBox.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkWidgetPickUpBox : public UDkWidgetActivatableBase
{
	GENERATED_BODY()

public:
	void ShowPickUpMessage(const FString& Message);
	void HidePickUpMessage();

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonTextBlock* CommonText_PickUpMessage;
	//***** Bound Widgets *****//
};
