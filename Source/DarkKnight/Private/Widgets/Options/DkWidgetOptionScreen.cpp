// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DkWidgetOptionScreen.h"

#include "DarkKnightDebugHelper.h"
#include "ICommonInputModule.h"
#include "Input/CommonUIInputTypes.h"
#include "Widgets/Components/DkUITabListWidgetBase.h"
#include "Widgets/Options/DkUIOptionsDataRegistry.h"
#include "Widgets/Options/DataObjects/DkUIListDataObjectCollection.h"

void UDkWidgetOptionScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (!ResetAction.IsNull())
	{
		ResetActionHandle = RegisterUIActionBinding(
			FBindUIActionArgs(
				ResetAction,
				true,
				FSimpleDelegate::CreateUObject(this, &ThisClass::OnResetBoundActionTriggered)
			)
		);
	}

	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true,
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)
		)
	);
}

void UDkWidgetOptionScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	for (UDkUIListDataObjectCollection* TabCollection : GetOrCreateDataRegistry()->GetRegisteredOptionsTabCollections())
	{
		if (!TabCollection)
		{
			continue;
		}

		if (TabListWidget_OptionsTabs->GetTabButtonBaseByID(TabCollection->GetDataID()) != nullptr)
		{
			continue;
		}

		TabListWidget_OptionsTabs->RequestRegisterTab(TabCollection->GetDataID(), TabCollection->GetDataDisplayName());
	}
}

UDkUIOptionsDataRegistry* UDkWidgetOptionScreen::GetOrCreateDataRegistry()
{
	if (!CreatedOwningDataRegistry)
	{
		CreatedOwningDataRegistry = NewObject<UDkUIOptionsDataRegistry>();
		CreatedOwningDataRegistry->InitOptionsDataRegister(GetOwningLocalPlayer());
	}

	checkf(CreatedOwningDataRegistry, TEXT("OptionScreen的OptionsDataRegistry无效"));
	return CreatedOwningDataRegistry;
}

void UDkWidgetOptionScreen::OnResetBoundActionTriggered()
{
	Debug::Print(TEXT("重置绑定按钮已触发"));
}

void UDkWidgetOptionScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}
