// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DkWidgetInfoMessage.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkWidgetInfoMessage : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void MessageShow();
	
	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void MessageHide();

	void SetMessage(const FText& Message);

protected:
	//~Begin UUserWidget
	virtual void NativeOnInitialized() override;
	//~End UUserWidget

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonTextBlock* CommonText_Message;
	//***** Bound Widgets *****//

	UPROPERTY(EditAnywhere, Category="Inventory")
	float MessageLifeTime = 3.f;

	bool bIsMessageActive = false;

	FTimerHandle MessageTimer;
};
