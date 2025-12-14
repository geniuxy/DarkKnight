// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkTypes/DkEnums.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "DkWidgetConfirmScreen.generated.h"

class UDynamicEntryBox;
class UCommonTextBlock;

USTRUCT(BlueprintType)
struct FConfirmScreenButtonInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EConfirmScreenButtonType ConfirmScreenButtonType = EConfirmScreenButtonType::Unknown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ButtonTextToDisplay;
};

UCLASS()
class DARKKNIGHT_API UConfirmScreenInfoObject : public UObject
{
	GENERATED_BODY()

public:
	static UConfirmScreenInfoObject* CreateOKScreen(const FText& InScreenTitle, const FText& InScreenMsg);
	static UConfirmScreenInfoObject* CreateYesOrNoScreen(const FText& InScreenTitle, const FText& InScreenMsg);
	static UConfirmScreenInfoObject* CreateOKOrCancelScreen(const FText& InScreenTitle, const FText& InScreenMsg);

	UPROPERTY(Transient)
	FText ScreenTitle;

	UPROPERTY(Transient)
	FText ScreenMessage;

	UPROPERTY(Transient)
	TArray<FConfirmScreenButtonInfo> AvailableScreenButtons;
};

/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DARKKNIGHT_API UDkWidgetConfirmScreen : public UDkWidgetActivatableBase
{
	GENERATED_BODY()

public:
	//~Begin UCommonActivatableWidget Function
	virtual UWidget* NativeGetDesiredFocusTarget() const override;
	//~End UCommonActivatableWidget Function
	
	// 此方法在类外部调用，调用时间点为该组件构造时、将其推入模态堆栈之前。
	void InitConfirmScreen(
		UConfirmScreenInfoObject* InScreenInfoObject, TFunction<void(EConfirmScreenButtonType)> ClickedButtonCallback
	);

private:
	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* CommonTextBlock_Title;

	UPROPERTY(meta=(BindWidget))
	UCommonTextBlock* CommonTextBlock_Message;

	UPROPERTY(meta=(BindWidget))
	UDynamicEntryBox* DynamicEntryBox_Buttons;
};
