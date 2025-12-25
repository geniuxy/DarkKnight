// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DkUIOptionsDataRegistry.h"

#include "DkGameplayTags.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Settings/DkGameUserSettings.h"
#include "Widgets/Options/DkUIOptionsDataInteractionHelper.h"
#include "Widgets/Options/DataObjects/DkUIListDataObjectCollection.h"
#include "Widgets/Options/DataObjects/DkUIListDataObjectString.h"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FDkUIOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UDkGameUserSettings, SetterOrGetterFuncName))

void UDkUIOptionsDataRegistry::InitOptionsDataRegister(ULocalPlayer* InOwningLocalPlayer)
{
	InitGameplayCollectionTab();
	InitAudioCollectionTab();
	InitVideoCollectionTab();
	InitControlCollectionTab();
}

TArray<UDkUIListDataObjectBase*> UDkUIOptionsDataRegistry::GetListSourceItemsBySelectedTabID(
	const FName& InSelectedTabID) const
{
	UDkUIListDataObjectCollection* const* FoundTabCollectionPtr = RegisteredOptionsTabCollections.FindByPredicate(
		[InSelectedTabID](UDkUIListDataObjectCollection* AvailableTabCollection)-> bool
		{
			return AvailableTabCollection->GetDataID() == InSelectedTabID;
		}
	);

	checkf(FoundTabCollectionPtr, TEXT("找不到有效的Option Tab Collection, 通过ID（%s）"), *InSelectedTabID.ToString());

	UDkUIListDataObjectCollection* FoundTabCollection = *FoundTabCollectionPtr;
	return FoundTabCollection->GetAllChildSettingData();
}

void UDkUIOptionsDataRegistry::InitGameplayCollectionTab()
{
	UDkUIListDataObjectCollection* GameplayTabCollection = NewObject<UDkUIListDataObjectCollection>();
	GameplayTabCollection->SetDataID(FName("GameplayTabCollection"));
	GameplayTabCollection->SetDataDisplayName(FText::FromString(TEXT("游戏性")));

	//以下是完整的代码用于构造DataInteractionHelper
	/*
	TSharedPtr<FOptionsDataInteractionHelper> ConstructedHelper = 
		MakeShared<FOptionsDataInteractionHelper>(
			GET_FUNCTION_NAME_STRING_CHECKED(UFrontendGameUserSettings,GetCurrentGameDifficulty)
		);
	*/

	// 游戏难度
	{
		UDkUIListDataObjectString* GameDifficulty = NewObject<UDkUIListDataObjectString>();
		GameDifficulty->SetDataID(FName("GameDifficulty"));
		GameDifficulty->SetDataDisplayName(FText::FromString(TEXT("游戏难度")));
		GameDifficulty->SetDescriptionRichText(FText::FromString(TEXT(
			"调整游戏体验的难易程度。\n\n"
			"<Bold>简单：</> 专注于故事体验。提供最轻松的战斗。\n\n"
			"<Bold>普通：</> 提供稍微困难一些的战斗体验\n\n"
			"<Bold>困难：</> 提供更具挑战性的战斗体验\n\n"
			"<Bold>非常困难：</> 提供最具挑战性的战斗体验，不推荐首次游玩选择。"
		)));
		GameDifficulty->AddDynamicOption(TEXT("Easy"), FText::FromString(TEXT("简单")));
		GameDifficulty->AddDynamicOption(TEXT("Normal"), FText::FromString(TEXT("正常")));
		GameDifficulty->AddDynamicOption(TEXT("Hard"), FText::FromString(TEXT("困难")));
		GameDifficulty->AddDynamicOption(TEXT("Very Hard"), FText::FromString(TEXT("非常困难")));
		GameDifficulty->SetDefaultValueFromString(TEXT("Normal"));
		GameDifficulty->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentGameDifficulty));
		GameDifficulty->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentGameDifficulty));
		GameDifficulty->SetShouldApplyChangeImmediately(true);

		GameplayTabCollection->AddChildListData(GameDifficulty);
	}

	// 显示模式
	{
		UDkUIListDataObjectString* DisplayMode = NewObject<UDkUIListDataObjectString>();
		DisplayMode->SetDataID(FName("DisplayMode"));
		DisplayMode->SetDataDisplayName(FText::FromString(TEXT("显示模式")));
		DisplayMode->SetSoftDescriptionImage(
			UDkUIFunctionLibrary::GetSoftImageByTag(DkGameplayTags::Dk_Image_TestImage)
		);
		DisplayMode->SetDescriptionRichText(FText::FromString(TEXT(
			"要显示的图像可以在项目设置中指定。它可以是开发人员在其中指定的任何图像。"
		)));

		GameplayTabCollection->AddChildListData(DisplayMode);
	}

	RegisteredOptionsTabCollections.Add(GameplayTabCollection);
}

void UDkUIOptionsDataRegistry::InitAudioCollectionTab()
{
	UDkUIListDataObjectCollection* AudioTabCollection = NewObject<UDkUIListDataObjectCollection>();
	AudioTabCollection->SetDataID(FName("AudioTabCollection"));
	AudioTabCollection->SetDataDisplayName(FText::FromString(TEXT("音频")));

	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

void UDkUIOptionsDataRegistry::InitVideoCollectionTab()
{
	UDkUIListDataObjectCollection* VideoTabCollection = NewObject<UDkUIListDataObjectCollection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString(TEXT("图像")));

	RegisteredOptionsTabCollections.Add(VideoTabCollection);
}

void UDkUIOptionsDataRegistry::InitControlCollectionTab()
{
	UDkUIListDataObjectCollection* ControlTabCollection = NewObject<UDkUIListDataObjectCollection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(FText::FromString(TEXT("控制")));

	RegisteredOptionsTabCollections.Add(ControlTabCollection);
}
