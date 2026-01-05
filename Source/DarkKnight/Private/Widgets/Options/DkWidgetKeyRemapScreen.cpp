// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DkWidgetKeyRemapScreen.h"

#include "DarkKnightDebugHelper.h"
#include "Framework/Application/IInputProcessor.h"

class FKeyRemapScreenInputPreprocessor : public IInputProcessor
{
protected:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
	{
		
	}

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		Debug::Print(TEXT("已按下键盘按键：") + InKeyEvent.GetKey().GetDisplayName().ToString());

		return true;
	}

	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		Debug::Print(TEXT("已按下鼠标按键：") + MouseEvent.GetEffectingButton().GetDisplayName().ToString());
		
		return true;
	}
};

void UDkWidgetKeyRemapScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	CachedInputPreprocessor = MakeShared<FKeyRemapScreenInputPreprocessor>();

	//数字越小越靠前，-1 表示“尽可能早地抢在所有默认预处理器之前拿到输入事件”
	FSlateApplication::Get().RegisterInputPreProcessor(CachedInputPreprocessor, -1);
}

void UDkWidgetKeyRemapScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	if (CachedInputPreprocessor)
	{
		FSlateApplication::Get().UnregisterInputPreProcessor(CachedInputPreprocessor);

		CachedInputPreprocessor.Reset();
	}
}
