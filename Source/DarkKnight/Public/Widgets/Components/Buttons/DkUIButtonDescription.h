// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DkUICommonButtonBase.h"
#include "DkUIButtonDescription.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkUIButtonDescription : public UCommonUserWidget
{
	GENERATED_BODY()
	
private:
	//~Begin UUserWidget UFunction
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	//~End UUserWidget UFunction
	
	//***** Bound Widgets *****//
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* CommonText_ButtonDescription;
	//***** Bound Widgets *****//
	
	UFUNCTION()
	void HandleDescriptionTextUpdated(UDkUICommonButtonBase* InButton, FText InDescriptionText);
};
