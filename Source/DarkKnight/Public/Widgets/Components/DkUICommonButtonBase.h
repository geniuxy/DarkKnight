// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonLazyImage.h"
#include "DkTypes/DkEnums.h"
#include "DkUICommonButtonBase.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class DARKKNIGHT_API UDkUICommonButtonBase : public UCommonButtonBase
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetButtonText(FText InText, EDkTextJustify InJustification = EDkTextJustify::Left);

	UFUNCTION(BlueprintCallable)
	FText GetButtonDisplayText() const;

	UFUNCTION(BlueprintCallable)
	void SetButtonDisplayImage(const FSlateBrush& InBrush);

	UFUNCTION(BlueprintCallable)
	void ToggleHighlightState(bool bShouldHighlight);
	
private:
	//~Begin UUserWidget UFunction
	virtual void NativePreConstruct() override;
	virtual void NativeOnInitialized() override;
	//~End UUserWidget UFunction

	//~Begin UUserWidget UFunction
	virtual void NativeOnCurrentTextStyleChanged() override;
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	//~End UUserWidget UFunction
	
	//***** Bound Widgets *****//
	UPROPERTY(meta=(BindWidgetOptional)) // 用于 C++ 与 UMG 蓝图之间的绑定
	UCommonTextBlock* CommonButtonTextBlock;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess = "true")) // 用于 C++ 与 UMG 蓝图之间的绑定
	UCommonLazyImage* CommonLazyImage_ButtonImage;
	//***** Bound Widgets *****//

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Button", meta=(AllowPrivateAccess="true"))
	FText ButtonDisplayText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Button", meta=(AllowPrivateAccess="true"))
	EDkTextJustify ButtonDisplayTextJustification = EDkTextJustify::Center;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Button", meta=(AllowPrivateAccess="true"))
	FText ButtonDescriptionText;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Button", meta=(AllowPrivateAccess="true"))
	bool bUseUpperCaseForButtonText = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Button", meta=(AllowPrivateAccess="true"))
	FLinearColor DefaultButtonImageColor = FLinearColor(0.4f,0.4f,0.4f,1.f);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI Button", meta=(AllowPrivateAccess="true"))
	FLinearColor HighlightButtonImageColor = FLinearColor(1.f,1.f, 1.f,1.f);
};
