// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DkWidgetInteractBox.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkWidgetInteractBox : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	void SetMessage(const FString& Message);

protected:
	//~Begin UUserWidget
	virtual void NativeOnInitialized() override;
	//~End UUserWidget

	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonTextBlock* CommonText_Message;
	//***** Bound Widgets *****//
};
