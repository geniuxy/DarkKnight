// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DkWidgetKeyRemapScreen.h"

#include "CommonInputSubsystem.h"
#include "CommonRichTextBlock.h"
#include "DarkKnightDebugHelper.h"
#include "ICommonInputModule.h"
#include "Framework/Application/IInputProcessor.h"
#include "CommonUITypes.h"

class FKeyRemapScreenInputPreprocessor : public IInputProcessor
{
public:
	FKeyRemapScreenInputPreprocessor(ECommonInputType InInputTypeToListenTo, ULocalPlayer* InLocalPlayer)
		: CachedInputTypeToListenTo(InInputTypeToListenTo), CachedWeakOwningLocalPlayer(InLocalPlayer)
	{
	}

	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeyPressedDelegate, const FKey& /* PressedKey */)
	FOnInputPreProcessorKeyPressedDelegate OnInputPreProcessorKeyPressed;
	DECLARE_DELEGATE_OneParam(FOnInputPreProcessorKeySelectCanceledDelegate, const FString& /* CanceledReason */)
	FOnInputPreProcessorKeySelectCanceledDelegate OnInputPreProcessorKeySelectCanceled;

protected:
	virtual void Tick(const float DeltaTime, FSlateApplication& SlateApp, TSharedRef<ICursor> Cursor) override
	{
	}

	virtual bool HandleKeyDownEvent(FSlateApplication& SlateApp, const FKeyEvent& InKeyEvent) override
	{
		ProcessPressedKey(InKeyEvent.GetKey());
		return true;
	}

	virtual bool HandleMouseButtonDownEvent(FSlateApplication& SlateApp, const FPointerEvent& MouseEvent) override
	{
		ProcessPressedKey(MouseEvent.GetEffectingButton());
		return true;
	}

	void ProcessPressedKey(const FKey& InPressedKey)
	{
		if (InPressedKey == EKeys::Escape)
		{
			OnInputPreProcessorKeySelectCanceled.ExecuteIfBound(TEXT("自定义按键操作已取消"));
			return;
		}

		UCommonInputSubsystem* CommonInputSubsystem = UCommonInputSubsystem::Get(CachedWeakOwningLocalPlayer.Get());

		check(CommonInputSubsystem);

		ECommonInputType CurrentInputType = CommonInputSubsystem->GetCurrentInputType();

		switch (CachedInputTypeToListenTo)
		{
		case ECommonInputType::MouseAndKeyboard:
			// 防止按下手柄的确定键，却误认为是鼠标左键(这是ue的bug)
			if (InPressedKey.IsGamepadKey() || CurrentInputType == ECommonInputType::Gamepad)
			{
				OnInputPreProcessorKeySelectCanceled.ExecuteIfBound(TEXT("检测到手柄按键按下，无法绑定到鼠标键盘按键，自定义按键操作已取消"));
				return;
			}
			break;
		case ECommonInputType::Gamepad:
			// 防止按下手柄的确定键，却误认为是鼠标左键(这是ue的bug)
			if (CurrentInputType == ECommonInputType::Gamepad && InPressedKey == EKeys::LeftMouseButton)
			{
				FCommonInputActionDataBase* InputActionData = ICommonInputModule::GetSettings().GetDefaultClickAction().
					GetRow<FCommonInputActionDataBase>(TEXT(""));

				check(InputActionData);

				OnInputPreProcessorKeyPressed.ExecuteIfBound(InputActionData->GetDefaultGamepadInputTypeInfo().GetKey());

				return;
			}

			if (!InPressedKey.IsGamepadKey())
			{
				OnInputPreProcessorKeySelectCanceled.ExecuteIfBound(TEXT("检测到非手柄按键按下，无法绑定到手柄按键，自定义按键操作已取消"));
				return;
			}
			break;
		default:
			break;
		}
		OnInputPreProcessorKeyPressed.ExecuteIfBound(InPressedKey);
	}

private:
	ECommonInputType CachedInputTypeToListenTo;
	TWeakObjectPtr<ULocalPlayer> CachedWeakOwningLocalPlayer;
};

void UDkWidgetKeyRemapScreen::SetDesiredInputTypeToFilter(ECommonInputType InDesiredInputType)
{
	CachedDesiredInputType = InDesiredInputType;
}

void UDkWidgetKeyRemapScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	CachedInputPreprocessor = MakeShared<FKeyRemapScreenInputPreprocessor>(
		CachedDesiredInputType, GetOwningLocalPlayer()
	);
	CachedInputPreprocessor->OnInputPreProcessorKeyPressed.BindUObject(this, &ThisClass::OnValidKeyPressedDetected);
	CachedInputPreprocessor->OnInputPreProcessorKeySelectCanceled.BindUObject(this, &ThisClass::OnKeySelectedCanceled);

	//数字越小越靠前，-1 表示“尽可能早地抢在所有默认预处理器之前拿到输入事件”
	FSlateApplication::Get().RegisterInputPreProcessor(CachedInputPreprocessor, -1);

	FString InputDeviceName;
	switch (CachedDesiredInputType)
	{
	case ECommonInputType::MouseAndKeyboard:
		InputDeviceName = TEXT("鼠标&键盘");
		break;
	case ECommonInputType::Gamepad:
		InputDeviceName = TEXT("手柄");
		break;
	default:
		break;
	}

	const FString DisplayRichMessage = FString::Printf(
		TEXT("<KeyRemapDefault>请按下想要绑定的</><KeyRemapHighlight>%s</><KeyRemapDefault>按键</>"), *InputDeviceName
	);

	CommonRichText_RemapMessage->SetText(FText::FromString(DisplayRichMessage));
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

void UDkWidgetKeyRemapScreen::OnValidKeyPressedDetected(const FKey& PressedKey)
{
	RequestDeactiveWidget(
		[this, PressedKey]()
		{
			OnKeyRemapScreenKeyPressed.ExecuteIfBound(PressedKey);
		}
	);
}

void UDkWidgetKeyRemapScreen::OnKeySelectedCanceled(const FString& CanceledReason)
{
	RequestDeactiveWidget(
		[this, CanceledReason]()
		{
			OnKeyRemapScreenKeySelectCanceled.ExecuteIfBound(CanceledReason);
		}
	);
}

void UDkWidgetKeyRemapScreen::RequestDeactiveWidget(TFunction<void()> PreDeactivateCallback)
{
	// 延迟一帧以确保输入被正确处理。
	// 注册一个只跑一次的 Tick，跑完就自尽，确保 PreDeactivateCallback 和 DeactivateWidget 都在下一帧执行，而不是立即执行。
	FTSTicker::GetCoreTicker().AddTicker(
		FTickerDelegate::CreateLambda(
			[this, PreDeactivateCallback](float DeltaTime)-> bool
			{
				PreDeactivateCallback();

				DeactivateWidget();

				return false; // false 表示从Tick列表里移除
			}
		)
	);
}
