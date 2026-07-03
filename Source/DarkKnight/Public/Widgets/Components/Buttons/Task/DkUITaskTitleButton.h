// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "DkUITaskTitleButton.generated.h"

class UCommonLazyImage;
class UCommonTextBlock;
class UBorder;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkUITaskTitleButton : public UCommonButtonBase
{
	GENERATED_BODY()

protected:
	//~Begin UUserWidget UFunction
	virtual void NativePreConstruct() override;
	//~End UUserWidget UFunction

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UBorder* TitleColor;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonTextBlock* TitleTextBlock;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonLazyImage* TitleOpenStatus;
	//***** Bound Widgets *****//

	UPROPERTY(EditAnywhere, Category="Button Details")
	FLinearColor TaskTypeColor;

	UPROPERTY(EditAnywhere, Category="Button Details")
	FText TitleText;
};
