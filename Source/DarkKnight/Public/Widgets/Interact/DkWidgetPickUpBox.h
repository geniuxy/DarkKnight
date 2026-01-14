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
class DARKKNIGHT_API UDkWidgetPickUpBox : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	void SetMessage(const FString& Message);

protected:
	//~Begin UUserWidget
	virtual void NativeOnInitialized() override;
	//~End UUserWidget

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonTextBlock* CommonText_PickUpMessage;
	//***** Bound Widgets *****//
};
