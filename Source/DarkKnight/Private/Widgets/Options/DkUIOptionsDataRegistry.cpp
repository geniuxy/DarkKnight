// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Options/DkUIOptionsDataRegistry.h"

#include "CommonNumericTextBlock.h"
#include "DkGameplayTags.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Settings/DkGameUserSettings.h"
#include "Widgets/Options/DkUIOptionsDataInteractionHelper.h"
#include "Widgets/Options/DataObjects/DkUIListDataObjectCollection.h"
#include "Widgets/Options/DataObjects/DkUIListDataObjectScalar.h"
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

	TArray<UDkUIListDataObjectBase*> AllChildSettingData;

	for (UDkUIListDataObjectBase* ChildListData : FoundTabCollection->GetAllChildSettingData())
	{
		if (!ChildListData)
		{
			continue;
		}

		AllChildSettingData.Add(ChildListData);

		if (ChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(ChildListData, AllChildSettingData);
		}
	}
	return AllChildSettingData;
}

void UDkUIOptionsDataRegistry::FindChildListDataRecursively(
	UDkUIListDataObjectBase* InParentData, TArray<UDkUIListDataObjectBase*>& OutFoundChildListData) const
{
	if (!InParentData || !InParentData->HasAnyChildListData())
	{
		return;
	}

	for (UDkUIListDataObjectBase* SubChildListData : InParentData->GetAllChildSettingData())
	{
		if (!SubChildListData)
		{
			continue;
		}

		OutFoundChildListData.Add(SubChildListData);

		if (SubChildListData->HasAnyChildListData())
		{
			FindChildListDataRecursively(SubChildListData, OutFoundChildListData);
		}
	}
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

	// 音量类别
	{
		UDkUIListDataObjectCollection* VolumeCategoryCollection = NewObject<UDkUIListDataObjectCollection>();
		VolumeCategoryCollection->SetDataID(FName("VolumeCategory"));
		VolumeCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("音量")));

		AudioTabCollection->AddChildListData(VolumeCategoryCollection);

		// 总音量
		{
			UDkUIListDataObjectScalar* OverallVolume = NewObject<UDkUIListDataObjectScalar>();
			OverallVolume->SetDataID(FName("OverallVolume"));
			OverallVolume->SetDataDisplayName(FText::FromString(TEXT("总音量")));
			OverallVolume->SetDescriptionRichText(FText::FromString(TEXT("调整总体的音量输出")));
			OverallVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			OverallVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			OverallVolume->SetSliderStepSize(0.01f);
			OverallVolume->SetDefaultValueFromString(LexToString(1.f));
			OverallVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			OverallVolume->SetNumberFormattingOptions(UDkUIListDataObjectScalar::NoDecimal());
			OverallVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallVolume));
			OverallVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallVolume));
			OverallVolume->SetShouldApplyChangeImmediately(true);

			VolumeCategoryCollection->AddChildListData(OverallVolume);
		}

		// 音乐大小
		{
			UDkUIListDataObjectScalar* MusicVolume = NewObject<UDkUIListDataObjectScalar>();
			MusicVolume->SetDataID(FName("MusicVolume"));
			MusicVolume->SetDataDisplayName(FText::FromString(TEXT("音乐大小")));
			MusicVolume->SetDescriptionRichText(FText::FromString(TEXT("调整游戏的音乐输出")));
			MusicVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			MusicVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			MusicVolume->SetSliderStepSize(0.01f);
			MusicVolume->SetDefaultValueFromString(LexToString(1.f));
			MusicVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			MusicVolume->SetNumberFormattingOptions(UDkUIListDataObjectScalar::NoDecimal());
			MusicVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetMusicVolume));
			MusicVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetMusicVolume));
			MusicVolume->SetShouldApplyChangeImmediately(true);

			VolumeCategoryCollection->AddChildListData(MusicVolume);
		}

		// 音效大小
		{
			UDkUIListDataObjectScalar* SoundFXVolume = NewObject<UDkUIListDataObjectScalar>();
			SoundFXVolume->SetDataID(FName("SoundFXVolume"));
			SoundFXVolume->SetDataDisplayName(FText::FromString(TEXT("音效大小")));
			SoundFXVolume->SetDescriptionRichText(FText::FromString(TEXT("调整游戏的音效大小")));
			SoundFXVolume->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			SoundFXVolume->SetOutputValueRange(TRange<float>(0.f, 2.f));
			SoundFXVolume->SetSliderStepSize(0.01f);
			SoundFXVolume->SetDefaultValueFromString(LexToString(1.f));
			SoundFXVolume->SetDisplayNumericType(ECommonNumericType::Percentage);
			SoundFXVolume->SetNumberFormattingOptions(UDkUIListDataObjectScalar::NoDecimal());
			SoundFXVolume->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetSoundFXVolume));
			SoundFXVolume->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetSoundFXVolume));
			SoundFXVolume->SetShouldApplyChangeImmediately(true);

			VolumeCategoryCollection->AddChildListData(SoundFXVolume);
		}
	}

	// 声音设置
	{
		UDkUIListDataObjectCollection* SoundCategoryCollection = NewObject<UDkUIListDataObjectCollection>();
		SoundCategoryCollection->SetDataID(FName("SoundCategory"));
		SoundCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("声音")));

		AudioTabCollection->AddChildListData(SoundCategoryCollection);

		// 允许后台播放声音
		{
			UDkUIListDataObjectStringBool* AllowBackgroundAudio = NewObject<UDkUIListDataObjectStringBool>();
			AllowBackgroundAudio->SetDataID(FName("AllowBackgroundAudio"));
			AllowBackgroundAudio->SetDataDisplayName(FText::FromString(TEXT("允许后台播放声音")));
			AllowBackgroundAudio->SetDescriptionRichText(FText::FromString(TEXT("决定是否允许游戏切换到后台之后继续播放声音")));
			AllowBackgroundAudio->OverrideTrueDisplayText(FText::FromString(TEXT("允许")));
			AllowBackgroundAudio->OverrideFalseDisplayText(FText::FromString(TEXT("不允许")));
			AllowBackgroundAudio->SetFalseAsDefaultValue();
			AllowBackgroundAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAllowBackgroundAudio));
			AllowBackgroundAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAllowBackgroundAudio));
			AllowBackgroundAudio->SetShouldApplyChangeImmediately(true);

			SoundCategoryCollection->AddChildListData(AllowBackgroundAudio);
		}
	}

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
