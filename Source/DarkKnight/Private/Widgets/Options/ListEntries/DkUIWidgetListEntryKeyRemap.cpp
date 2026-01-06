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
	Debug::Print(TEXT("Reset Key Binding Button Clicked"));
}

void UDkUIWidgetListEntryKeyRemap::OnKeyToRemapPressed(const FKey& PressedKey)
{
	Debug::Print(TEXT("按下的按键为: ") + PressedKey.GetDisplayName().ToString());
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
