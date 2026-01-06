// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/ListEntries/DkUIWidgetListEntryKeyRemap.h"

#include "DarkKnightDebugHelper.h"
#include "DkGameplayTags.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Subsytems/DkUISubsystem.h"
#include "Widgets/Components/DkUICommonButtonBase.h"
#include "Widgets/Options/DkWidgetKeyRemapScreen.h"
#include "Widgets/Options/DataObjects/DkUIListDataObjectKeyRemap.h"

void UDkUIWidgetListEntryKeyRemap::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	CommonButton_RemapKey->OnClicked().AddUObject(this, &ThisClass::OnRemapKeyButtonClicked);
	CommonButton_ResetKeyBinding->OnClicked().AddUObject(this, &ThisClass::OnResetKeyBindingButtonClicked);
}

void UDkUIWidgetListEntryKeyRemap::OnOwningListDataObjectSet(UDkUIListDataObjectBase* InOwningListDataObject)
{
	Super::OnOwningListDataObjectSet(InOwningListDataObject);

	CachedOwningKeyRemapDataObject = CastChecked<UDkUIListDataObjectKeyRemap>(InOwningListDataObject);

	CommonButton_RemapKey->SetButtonDisplayImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
}

void UDkUIWidgetListEntryKeyRemap::OnOwningListDataObjectModified(
	UDkUIListDataObjectBase* OwningModifiedData, EOptionsListDataModifyReason ModifyReason)
{
	if (CachedOwningKeyRemapDataObject)
	{
		CommonButton_RemapKey->SetButtonDisplayImage(CachedOwningKeyRemapDataObject->GetIconFromCurrentKey());
	}
}

void UDkUIWidgetListEntryKeyRemap::OnRemapKeyButtonClicked()
{
	SelectThisEntryWidget();

	UDkUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
		DkGameplayTags::Dk_WidgetStack_Modal,
		UDkUIFunctionLibrary::GetUISoftWidgetClassByTag(DkGameplayTags::Dk_Widget_KeyRemapScreen),
		[this](EAsyncPushWidgetState InPushState, UDkWidgetActivatableBase* PushedWidget)
		{
			if (InPushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UDkWidgetKeyRemapScreen* CreatedKeyRemapScreen = CastChecked<UDkWidgetKeyRemapScreen>(PushedWidget);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeyPressed.BindUObject(
					this, &ThisClass::OnKeyToRemapPressed
				);
				CreatedKeyRemapScreen->OnKeyRemapScreenKeySelectCanceled.BindUObject(
					this, &ThisClass::OnKeyRemapCanceled
				);

				if (CachedOwningKeyRemapDataObject)
				{
					CreatedKeyRemapScreen->SetDesiredInputTypeToFilter(
						CachedOwningKeyRemapDataObject->GetDesiredInputKeyType()
					);
				}
			}
		}
	);
}

void UDkUIWidgetListEntryKeyRemap::OnResetKeyBindingButtonClicked()
{
	SelectThisEntryWidget();

	if (!CachedOwningKeyRemapDataObject)
	{
		return;
	}

	// 检查当前按键是否已是默认按键。如果已是默认按键，则向玩家显示“确定”屏幕，提示该按键已是默认按键。
	if (!CachedOwningKeyRemapDataObject->CanResetBackToDefaultValue())
	{
		UDkUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
			EConfirmScreenType::Ok,
			FText::FromString(TEXT("重置控制按键")),
			FText::FromString(TEXT("当前按键已是默认按键")),
			[](EConfirmScreenButtonType ClickedButtonType)
			{
			}
		);

		return;
	}

	// 将按键绑定重置为默认值
	UDkUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmScreenType::YesOrNo,
		FText::FromString(TEXT("重置控制按键")),
		FText::FromString(
			TEXT("你想要重置 ") + CachedOwningKeyRemapDataObject->GetDataDisplayName().ToString() + TEXT(" 所绑定的按键吗?")
		),
		[this](EConfirmScreenButtonType ClickedButtonType)
		{
			if (ClickedButtonType == EConfirmScreenButtonType::Confirmed)
			{
				CachedOwningKeyRemapDataObject->TryResetBackToDefaultValue();
			}
		}
	);
}

void UDkUIWidgetListEntryKeyRemap::OnKeyToRemapPressed(const FKey& PressedKey)
{
	if (CachedOwningKeyRemapDataObject)
	{
		CachedOwningKeyRemapDataObject->BindNewInputKey(PressedKey);
	}
}

void UDkUIWidgetListEntryKeyRemap::OnKeyRemapCanceled(const FString& CanceledReason)
{
	UDkUISubsystem::Get(this)->PushConfirmScreenToModalStackAsync(
		EConfirmScreenType::Ok,
		FText::FromString(TEXT("自定义按键取消")),
		FText::FromString(CanceledReason),
		[](EConfirmScreenButtonType ClickedButtonType)
		{
		}
	);
}
