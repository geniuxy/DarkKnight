// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameMenu/ShopMenu/DkWidgetShopMenuScreen.h"

#include "Components/DkNpcDialogComponent.h"
#include "DarkKnightDebugHelper.h"
#include "ICommonInputModule.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Input/CommonUIInputTypes.h"
#include "Subsytems/EngineSubsystems/DkDataSubsystem.h"

void UDkWidgetShopMenuScreen::ConfigureShopMenu(int InNpcId, int InDialogId)
{
	CurNpcId = InNpcId;
	NextDialogId = InDialogId;
}

void UDkWidgetShopMenuScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	RegisterUIActionBinding(
		FBindUIActionArgs(
			ICommonInputModule::GetSettings().GetDefaultBackAction(),
			true,
			FSimpleDelegate::CreateUObject(this, &ThisClass::OnBackBoundActionTriggered)
		)
	);
}

void UDkWidgetShopMenuScreen::NativeOnActivated()
{
	Super::NativeOnActivated();
}

void UDkWidgetShopMenuScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	if (!GetNpcDialogComponent()) return;

	CachedNpcDialogComponent->OnTriggerNextDialog.Broadcast(NextDialogId);
}

UDkNpcDialogComponent* UDkWidgetShopMenuScreen::GetNpcDialogComponent()
{
	if (!IsValid(CachedNpcDialogComponent))
	{
		AActor* CurNpcActor = UDkDataSubsystem::Get()->GetNpcInfo().FindRef(CurNpcId).NpcActor;
		if (!CurNpcActor)
		{
			Debug::Print(FString::Printf(TEXT("获取商店对应NpcDialogComp时，没找到对应的Npc Actor, Id为: %d"), CurNpcId));
			return nullptr;
		}

		CachedNpcDialogComponent = CurNpcActor->FindComponentByClass<UDkNpcDialogComponent>();
	}

	return CachedNpcDialogComponent;
}

void UDkWidgetShopMenuScreen::OnBackBoundActionTriggered()
{
	DeactivateWidget();
}
