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
#include "Widgets/Options/DataObjects/DkUIListDataObjectStrResolution.h"
#include "Internationalization/StringTableRegistry.h"

#define MAKE_OPTIONS_DATA_CONTROL(SetterOrGetterFuncName) \
	MakeShared<FDkUIOptionsDataInteractionHelper>(GET_FUNCTION_NAME_STRING_CHECKED(UDkGameUserSettings, SetterOrGetterFuncName))

#define GET_DESCRIPTION(InKey) \
	LOCTABLE("/Game/_Blueprints/UI/StringTable/ST_OptionScreenDescription.ST_OptionScreenDescription", InKey)

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
		GameDifficulty->SetDescriptionRichText(GET_DESCRIPTION("GameDifficultyDescKey"));
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

	// Test
	{
		UDkUIListDataObjectString* TestItem = NewObject<UDkUIListDataObjectString>();
		TestItem->SetDataID(FName("TestItem"));
		TestItem->SetDataDisplayName(FText::FromString(TEXT("测试项")));
		TestItem->SetSoftDescriptionImage(
			UDkUIFunctionLibrary::GetSoftImageByTag(DkGameplayTags::Dk_Image_TestImage)
		);
		TestItem->SetDescriptionRichText(GET_DESCRIPTION("TestItemDescKey"));

		GameplayTabCollection->AddChildListData(TestItem);
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
			OverallVolume->SetDescriptionRichText(GET_DESCRIPTION("OverallVolumeDescKey"));
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
			MusicVolume->SetDescriptionRichText(GET_DESCRIPTION("MusicVolumeDescKey"));
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
			SoundFXVolume->SetDescriptionRichText(GET_DESCRIPTION("SoundFXVolumeDescKey"));
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
			AllowBackgroundAudio->SetDescriptionRichText(GET_DESCRIPTION("AllowBackgroundAudioDescKey"));
			AllowBackgroundAudio->OverrideTrueDisplayText(FText::FromString(TEXT("允许")));
			AllowBackgroundAudio->OverrideFalseDisplayText(FText::FromString(TEXT("不允许")));
			AllowBackgroundAudio->SetFalseAsDefaultValue();
			AllowBackgroundAudio->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAllowBackgroundAudio));
			AllowBackgroundAudio->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAllowBackgroundAudio));
			AllowBackgroundAudio->SetShouldApplyChangeImmediately(true);

			SoundCategoryCollection->AddChildListData(AllowBackgroundAudio);
		}

		// 开启高动态音频
		{
			UDkUIListDataObjectStringBool* UseHDRAudioMode = NewObject<UDkUIListDataObjectStringBool>();
			UseHDRAudioMode->SetDataID(FName("UseHDRAudioMode"));
			UseHDRAudioMode->SetDataDisplayName(FText::FromString(TEXT("开启高动态音频")));
			UseHDRAudioMode->SetDescriptionRichText(GET_DESCRIPTION("UseHDRAudioModeDescKey"));
			UseHDRAudioMode->SetFalseAsDefaultValue();
			UseHDRAudioMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetUseHDRAudioMode));
			UseHDRAudioMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetUseHDRAudioMode));
			UseHDRAudioMode->SetShouldApplyChangeImmediately(true);

			SoundCategoryCollection->AddChildListData(UseHDRAudioMode);
		}
	}

	RegisteredOptionsTabCollections.Add(AudioTabCollection);
}

void UDkUIOptionsDataRegistry::InitVideoCollectionTab()
{
	UDkUIListDataObjectCollection* VideoTabCollection = NewObject<UDkUIListDataObjectCollection>();
	VideoTabCollection->SetDataID(FName("VideoTabCollection"));
	VideoTabCollection->SetDataDisplayName(FText::FromString(TEXT("图像")));

	FOptionsDataEditConditionDescriptor PackagedBuildOnlyCondition;
	PackagedBuildOnlyCondition.SetEditConditionFunc(
		[]()-> bool
		{
			const bool bIsInEditor = GIsEditor || GIsPlayInEditorWorld;
			return !bIsInEditor;
		}
	);
	PackagedBuildOnlyCondition.SetDisabledWarningReason(TEXT("\n\n<Warning>该设置仅在打包客户端中可编辑</>"));

	UDkUIListDataObjectStringEnum* CreatedWindowMode = nullptr;

	// 显示类别
	{
		UDkUIListDataObjectCollection* DisplayCategoryCollection = NewObject<UDkUIListDataObjectCollection>();
		DisplayCategoryCollection->SetDataID(FName("DisplayCategory"));
		DisplayCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("显示")));

		VideoTabCollection->AddChildListData(DisplayCategoryCollection);

		// 窗口模式
		{
			UDkUIListDataObjectStringEnum* WindowMode = NewObject<UDkUIListDataObjectStringEnum>();
			WindowMode->SetDataID(FName("WindowMode"));
			WindowMode->SetDataDisplayName(FText::FromString(TEXT("窗口模式")));
			WindowMode->SetSoftDescriptionImage(
				UDkUIFunctionLibrary::GetSoftImageByTag(DkGameplayTags::Dk_Image_TestImage)
			);
			WindowMode->SetDescriptionRichText(GET_DESCRIPTION("WindowModeDescKey"));
			WindowMode->AddEnumOption(EWindowMode::Fullscreen, FText::FromString(TEXT("全屏")));
			WindowMode->AddEnumOption(EWindowMode::WindowedFullscreen, FText::FromString(TEXT("无边框窗口")));
			WindowMode->AddEnumOption(EWindowMode::Windowed, FText::FromString(TEXT("窗口化")));
			WindowMode->SetDefaultValueFromEnumOption(EWindowMode::WindowedFullscreen);
			WindowMode->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFullscreenMode));
			WindowMode->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFullscreenMode));
			WindowMode->SetShouldApplyChangeImmediately(true);
			WindowMode->AddEditionCondition(PackagedBuildOnlyCondition);

			CreatedWindowMode = WindowMode;

			DisplayCategoryCollection->AddChildListData(WindowMode);
		}

		// 屏幕分辨率
		{
			UDkUIListDataObjectStrResolution* ScreenResolution = NewObject<UDkUIListDataObjectStrResolution>();
			ScreenResolution->SetDataID(FName("ScreenResolution"));
			ScreenResolution->SetDataDisplayName(FText::FromString(TEXT("分辨率")));
			ScreenResolution->SetDescriptionRichText(GET_DESCRIPTION("ScreenResolutionDescKey"));
			ScreenResolution->InitResolutionValues();
			ScreenResolution->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetScreenResolution));
			ScreenResolution->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetScreenResolution));
			ScreenResolution->SetShouldApplyChangeImmediately(true);
			ScreenResolution->AddEditionCondition(PackagedBuildOnlyCondition);

			FOptionsDataEditConditionDescriptor WindowModeEditCondition;
			WindowModeEditCondition.SetEditConditionFunc(
				[CreatedWindowMode]()-> bool
				{
					const bool bIsBorderlessWindow = CreatedWindowMode->
						GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::WindowedFullscreen;
					return !bIsBorderlessWindow;
				}
			);
			WindowModeEditCondition.SetDisabledWarningReason(
				TEXT("\n\n<Warning>当无边框窗口模式时，无法调节屏幕分辨率，默认会设置为当前屏幕最大允许分辨率。</>")
			);
			WindowModeEditCondition.SetDisabledForcedStringValue(ScreenResolution->GetMaxAllowedResolution());
			ScreenResolution->AddEditionCondition(WindowModeEditCondition);

			ScreenResolution->AddEditionDependencyData(CreatedWindowMode);

			DisplayCategoryCollection->AddChildListData(ScreenResolution);
		}
	}

	// 图形类别
	{
		UDkUIListDataObjectCollection* GraphicsCategoryCollection = NewObject<UDkUIListDataObjectCollection>();
		GraphicsCategoryCollection->SetDataID(FName("GraphicsCategory"));
		GraphicsCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("图形")));

		VideoTabCollection->AddChildListData(GraphicsCategoryCollection);

		// 显示亮度(Display Gamma)
		{
			UDkUIListDataObjectScalar* DisplayGamma = NewObject<UDkUIListDataObjectScalar>();
			DisplayGamma->SetDataID(FName("DisplayGamma"));
			DisplayGamma->SetDataDisplayName(FText::FromString(TEXT("亮度")));
			DisplayGamma->SetDescriptionRichText(GET_DESCRIPTION("DisplayGammaDescKey"));
			DisplayGamma->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			DisplayGamma->SetOutputValueRange(TRange<float>(1.7f, 2.7f)); // 虚幻默认的Gamma值为2.2f
			DisplayGamma->SetSliderStepSize(0.025f);
			DisplayGamma->SetDefaultValueFromString(LexToString(2.2f));
			DisplayGamma->SetDisplayNumericType(ECommonNumericType::Percentage);
			DisplayGamma->SetNumberFormattingOptions(UDkUIListDataObjectScalar::NoDecimal());
			DisplayGamma->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetCurrentDisplayGamma));
			DisplayGamma->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetCurrentDisplayGamma));
			DisplayGamma->SetShouldApplyChangeImmediately(true);

			GraphicsCategoryCollection->AddChildListData(DisplayGamma);
		}

		UDkUIListDataObjectStringInteger* CreatedOverallQuality = nullptr;

		// 整体质量
		{
			UDkUIListDataObjectStringInteger* OverallQuality = NewObject<UDkUIListDataObjectStringInteger>();
			OverallQuality->SetDataID(FName("OverallQuality"));
			OverallQuality->SetDataDisplayName(FText::FromString(TEXT("整体质量")));
			OverallQuality->SetDescriptionRichText(GET_DESCRIPTION("OverallQualityDescKey"));
			OverallQuality->AddIntegerOption(0, FText::FromString(TEXT("低")));
			OverallQuality->AddIntegerOption(1, FText::FromString(TEXT("中")));
			OverallQuality->AddIntegerOption(2, FText::FromString(TEXT("高")));
			OverallQuality->AddIntegerOption(3, FText::FromString(TEXT("极高")));
			OverallQuality->AddIntegerOption(4, FText::FromString(TEXT("最佳")));
			OverallQuality->SetDefaultValueFromString(LexToString(3));
			OverallQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetOverallScalabilityLevel));
			OverallQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetOverallScalabilityLevel));
			OverallQuality->SetShouldApplyChangeImmediately(true);

			CreatedOverallQuality = OverallQuality;
		}

		// 渲染比例
		{
			UDkUIListDataObjectScalar* ResolutionScale = NewObject<UDkUIListDataObjectScalar>();
			ResolutionScale->SetDataID(FName("ResolutionScale"));
			ResolutionScale->SetDataDisplayName(FText::FromString(TEXT("渲染比例")));
			ResolutionScale->SetDescriptionRichText(GET_DESCRIPTION("ResolutionScaleDescKey"));
			ResolutionScale->SetDisplayValueRange(TRange<float>(0.f, 1.f));
			ResolutionScale->SetOutputValueRange(TRange<float>(0.f, 1.f));
			ResolutionScale->SetSliderStepSize(0.01f);
			ResolutionScale->SetDisplayNumericType(ECommonNumericType::Percentage);
			ResolutionScale->SetNumberFormattingOptions(UDkUIListDataObjectScalar::NoDecimal());
			ResolutionScale->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetResolutionScaleNormalized));
			ResolutionScale->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetResolutionScaleNormalized));
			ResolutionScale->SetShouldApplyChangeImmediately(true);

			ResolutionScale->AddEditionDependencyData(CreatedOverallQuality);

			GraphicsCategoryCollection->AddChildListData(ResolutionScale);
		}

		GraphicsCategoryCollection->AddChildListData(CreatedOverallQuality); // 为了将整体质量放到渲染比例后面显示

		// 全局光照质量
		{
			UDkUIListDataObjectStringInteger* GlobalIlluminationQuality = NewObject<UDkUIListDataObjectStringInteger>();
			GlobalIlluminationQuality->SetDataID(FName("GlobalIlluminationQuality"));
			GlobalIlluminationQuality->SetDataDisplayName(FText::FromString(TEXT("全局光照")));
			GlobalIlluminationQuality->SetDescriptionRichText(GET_DESCRIPTION("GlobalIlluminationQualityDescKey"));
			GlobalIlluminationQuality->AddIntegerOption(0, FText::FromString(TEXT("低")));
			GlobalIlluminationQuality->AddIntegerOption(1, FText::FromString(TEXT("中")));
			GlobalIlluminationQuality->AddIntegerOption(2, FText::FromString(TEXT("高")));
			GlobalIlluminationQuality->AddIntegerOption(3, FText::FromString(TEXT("极高")));
			GlobalIlluminationQuality->AddIntegerOption(4, FText::FromString(TEXT("最佳")));
			GlobalIlluminationQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetGlobalIlluminationQuality));
			GlobalIlluminationQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetGlobalIlluminationQuality));
			GlobalIlluminationQuality->SetShouldApplyChangeImmediately(true);

			GlobalIlluminationQuality->AddEditionDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditionDependencyData(GlobalIlluminationQuality);

			GraphicsCategoryCollection->AddChildListData(GlobalIlluminationQuality);
		}

		// 阴影质量
		{
			UDkUIListDataObjectStringInteger* ShadowQuality = NewObject<UDkUIListDataObjectStringInteger>();
			ShadowQuality->SetDataID(FName("ShadowQuality"));
			ShadowQuality->SetDataDisplayName(FText::FromString(TEXT("阴影")));
			ShadowQuality->SetDescriptionRichText(GET_DESCRIPTION("ShadowQualityDescKey"));
			ShadowQuality->AddIntegerOption(0, FText::FromString(TEXT("低")));
			ShadowQuality->AddIntegerOption(1, FText::FromString(TEXT("中")));
			ShadowQuality->AddIntegerOption(2, FText::FromString(TEXT("高")));
			ShadowQuality->AddIntegerOption(3, FText::FromString(TEXT("极高")));
			ShadowQuality->AddIntegerOption(4, FText::FromString(TEXT("最佳")));
			ShadowQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetShadowQuality));
			ShadowQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetShadowQuality));
			ShadowQuality->SetShouldApplyChangeImmediately(true);

			ShadowQuality->AddEditionDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditionDependencyData(ShadowQuality);

			GraphicsCategoryCollection->AddChildListData(ShadowQuality);
		}

		// 抗锯齿质量
		{
			UDkUIListDataObjectStringInteger* AntiAliasingQuality = NewObject<UDkUIListDataObjectStringInteger>();
			AntiAliasingQuality->SetDataID(FName("AntiAliasingQuality"));
			AntiAliasingQuality->SetDataDisplayName(FText::FromString(TEXT("抗锯齿")));
			AntiAliasingQuality->SetDescriptionRichText(GET_DESCRIPTION("AntiAliasingQualityDescKey"));
			AntiAliasingQuality->AddIntegerOption(0, FText::FromString(TEXT("低")));
			AntiAliasingQuality->AddIntegerOption(1, FText::FromString(TEXT("中")));
			AntiAliasingQuality->AddIntegerOption(2, FText::FromString(TEXT("高")));
			AntiAliasingQuality->AddIntegerOption(3, FText::FromString(TEXT("极高")));
			AntiAliasingQuality->AddIntegerOption(4, FText::FromString(TEXT("最佳")));
			AntiAliasingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetAntiAliasingQuality));
			AntiAliasingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetAntiAliasingQuality));
			AntiAliasingQuality->SetShouldApplyChangeImmediately(true);

			AntiAliasingQuality->AddEditionDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditionDependencyData(AntiAliasingQuality);

			GraphicsCategoryCollection->AddChildListData(AntiAliasingQuality);
		}

		// 可视距离
		{
			UDkUIListDataObjectStringInteger* ViewDistanceQuality = NewObject<UDkUIListDataObjectStringInteger>();
			ViewDistanceQuality->SetDataID(FName("ViewDistanceQuality"));
			ViewDistanceQuality->SetDataDisplayName(FText::FromString(TEXT("可视距离")));
			ViewDistanceQuality->SetDescriptionRichText(GET_DESCRIPTION("ViewDistanceQualityDescKey"));
			ViewDistanceQuality->AddIntegerOption(0, FText::FromString(TEXT("近")));
			ViewDistanceQuality->AddIntegerOption(1, FText::FromString(TEXT("中")));
			ViewDistanceQuality->AddIntegerOption(2, FText::FromString(TEXT("远")));
			ViewDistanceQuality->AddIntegerOption(3, FText::FromString(TEXT("极远")));
			ViewDistanceQuality->AddIntegerOption(4, FText::FromString(TEXT("真实")));
			ViewDistanceQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetViewDistanceQuality));
			ViewDistanceQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetViewDistanceQuality));
			ViewDistanceQuality->SetShouldApplyChangeImmediately(true);

			ViewDistanceQuality->AddEditionDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditionDependencyData(ViewDistanceQuality);

			GraphicsCategoryCollection->AddChildListData(ViewDistanceQuality);
		}

		// 纹理质量
		{
			UDkUIListDataObjectStringInteger* TextureQuality = NewObject<UDkUIListDataObjectStringInteger>();
			TextureQuality->SetDataID(FName("TextureQuality"));
			TextureQuality->SetDataDisplayName(FText::FromString(TEXT("纹理")));
			TextureQuality->SetDescriptionRichText(GET_DESCRIPTION("TextureQualityDescKey"));
			TextureQuality->AddIntegerOption(0, FText::FromString(TEXT("低")));
			TextureQuality->AddIntegerOption(1, FText::FromString(TEXT("中")));
			TextureQuality->AddIntegerOption(2, FText::FromString(TEXT("高")));
			TextureQuality->AddIntegerOption(3, FText::FromString(TEXT("极高")));
			TextureQuality->AddIntegerOption(4, FText::FromString(TEXT("最佳")));
			TextureQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetTextureQuality));
			TextureQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetTextureQuality));
			TextureQuality->SetShouldApplyChangeImmediately(true);

			TextureQuality->AddEditionDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditionDependencyData(TextureQuality);

			GraphicsCategoryCollection->AddChildListData(TextureQuality);
		}

		// 特效质量
		{
			UDkUIListDataObjectStringInteger* VisualEffectQuality = NewObject<UDkUIListDataObjectStringInteger>();
			VisualEffectQuality->SetDataID(FName("VisualEffectQuality"));
			VisualEffectQuality->SetDataDisplayName(FText::FromString(TEXT("特效")));
			VisualEffectQuality->SetDescriptionRichText(GET_DESCRIPTION("VisualEffectQualityDescKey"));
			VisualEffectQuality->AddIntegerOption(0, FText::FromString(TEXT("低")));
			VisualEffectQuality->AddIntegerOption(1, FText::FromString(TEXT("中")));
			VisualEffectQuality->AddIntegerOption(2, FText::FromString(TEXT("高")));
			VisualEffectQuality->AddIntegerOption(3, FText::FromString(TEXT("极高")));
			VisualEffectQuality->AddIntegerOption(4, FText::FromString(TEXT("最佳")));
			VisualEffectQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetVisualEffectQuality));
			VisualEffectQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVisualEffectQuality));
			VisualEffectQuality->SetShouldApplyChangeImmediately(true);

			VisualEffectQuality->AddEditionDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditionDependencyData(VisualEffectQuality);

			GraphicsCategoryCollection->AddChildListData(VisualEffectQuality);
		}

		// 反射质量
		{
			UDkUIListDataObjectStringInteger* ReflectionQuality = NewObject<UDkUIListDataObjectStringInteger>();
			ReflectionQuality->SetDataID(FName("ReflectionQuality"));
			ReflectionQuality->SetDataDisplayName(FText::FromString(TEXT("反射")));
			ReflectionQuality->SetDescriptionRichText(GET_DESCRIPTION("ReflectionQualityDescKey"));
			ReflectionQuality->AddIntegerOption(0, FText::FromString(TEXT("低")));
			ReflectionQuality->AddIntegerOption(1, FText::FromString(TEXT("中")));
			ReflectionQuality->AddIntegerOption(2, FText::FromString(TEXT("高")));
			ReflectionQuality->AddIntegerOption(3, FText::FromString(TEXT("极高")));
			ReflectionQuality->AddIntegerOption(4, FText::FromString(TEXT("最佳")));
			ReflectionQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetReflectionQuality));
			ReflectionQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetReflectionQuality));
			ReflectionQuality->SetShouldApplyChangeImmediately(true);

			ReflectionQuality->AddEditionDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditionDependencyData(ReflectionQuality);

			GraphicsCategoryCollection->AddChildListData(ReflectionQuality);
		}

		// 后处理质量
		{
			UDkUIListDataObjectStringInteger* PostProcessingQuality = NewObject<UDkUIListDataObjectStringInteger>();
			PostProcessingQuality->SetDataID(FName("PostProcessingQuality"));
			PostProcessingQuality->SetDataDisplayName(FText::FromString(TEXT("后处理")));
			PostProcessingQuality->SetDescriptionRichText(GET_DESCRIPTION("PostProcessingQualityDescKey"));
			PostProcessingQuality->AddIntegerOption(0, FText::FromString(TEXT("低")));
			PostProcessingQuality->AddIntegerOption(1, FText::FromString(TEXT("中")));
			PostProcessingQuality->AddIntegerOption(2, FText::FromString(TEXT("高")));
			PostProcessingQuality->AddIntegerOption(3, FText::FromString(TEXT("极高")));
			PostProcessingQuality->AddIntegerOption(4, FText::FromString(TEXT("最佳")));
			PostProcessingQuality->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetPostProcessingQuality));
			PostProcessingQuality->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetPostProcessingQuality));
			PostProcessingQuality->SetShouldApplyChangeImmediately(true);

			PostProcessingQuality->AddEditionDependencyData(CreatedOverallQuality);

			CreatedOverallQuality->AddEditionDependencyData(PostProcessingQuality);

			GraphicsCategoryCollection->AddChildListData(PostProcessingQuality);
		}
	}

	// 高级类别
	{
		UDkUIListDataObjectCollection* AdvancedGraphicsCategoryCollection = NewObject<UDkUIListDataObjectCollection>();
		AdvancedGraphicsCategoryCollection->SetDataID(FName("GraphicsCategory"));
		AdvancedGraphicsCategoryCollection->SetDataDisplayName(FText::FromString(TEXT("高级")));

		VideoTabCollection->AddChildListData(AdvancedGraphicsCategoryCollection);

		// 垂直同步
		{
			UDkUIListDataObjectStringBool* VerticalSync = NewObject<UDkUIListDataObjectStringBool>();
			VerticalSync->SetDataID(FName("VerticalSync"));
			VerticalSync->SetDataDisplayName(FText::FromString(TEXT("垂直同步")));
			VerticalSync->SetDescriptionRichText(GET_DESCRIPTION("VerticalSyncDescKey"));
			VerticalSync->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(IsVSyncEnabled));
			VerticalSync->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetVSyncEnabled));
			VerticalSync->SetFalseAsDefaultValue();
			VerticalSync->SetShouldApplyChangeImmediately(true);

			FOptionsDataEditConditionDescriptor FullscreenOnlyCondition;
			FullscreenOnlyCondition.SetEditConditionFunc(
				[CreatedWindowMode]()-> bool
				{
					return CreatedWindowMode->GetCurrentValueAsEnum<EWindowMode::Type>() == EWindowMode::Fullscreen;
				}
			);
			FullscreenOnlyCondition.SetDisabledWarningReason(TEXT("\n\n<Warning>只有在全屏模式时，才能启用垂直同步。</>"));
			FullscreenOnlyCondition.SetDisabledForcedStringValue(TEXT("false"));

			VerticalSync->AddEditionCondition(FullscreenOnlyCondition);

			AdvancedGraphicsCategoryCollection->AddChildListData(VerticalSync);
		}

		// 帧率限制
		{
			UDkUIListDataObjectString* FrameRateLimit = NewObject<UDkUIListDataObjectString>();
			FrameRateLimit->SetDataID(FName("FrameRateLimit"));
			FrameRateLimit->SetDataDisplayName(FText::FromString(TEXT("帧率上限")));
			FrameRateLimit->SetDescriptionRichText(GET_DESCRIPTION("FrameRateLimitDescKey"));
			FrameRateLimit->AddDynamicOption(LexToString(30.f), FText::FromString(TEXT("30 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(60.f), FText::FromString(TEXT("60 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(90.f), FText::FromString(TEXT("90 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(120.f), FText::FromString(TEXT("120 FPS")));
			FrameRateLimit->AddDynamicOption(LexToString(0.f), FText::FromString(TEXT("无限制")));
			FrameRateLimit->SetDefaultValueFromString(LexToString(0.f));
			FrameRateLimit->SetDataDynamicGetter(MAKE_OPTIONS_DATA_CONTROL(GetFrameRateLimit));
			FrameRateLimit->SetDataDynamicSetter(MAKE_OPTIONS_DATA_CONTROL(SetFrameRateLimit));
			FrameRateLimit->SetShouldApplyChangeImmediately(true);

			AdvancedGraphicsCategoryCollection->AddChildListData(FrameRateLimit);
		}
	}

	RegisteredOptionsTabCollections.Add(VideoTabCollection);
}

void UDkUIOptionsDataRegistry::InitControlCollectionTab()
{
	UDkUIListDataObjectCollection* ControlTabCollection = NewObject<UDkUIListDataObjectCollection>();
	ControlTabCollection->SetDataID(FName("ControlTabCollection"));
	ControlTabCollection->SetDataDisplayName(FText::FromString(TEXT("控制")));

	RegisteredOptionsTabCollections.Add(ControlTabCollection);
}
