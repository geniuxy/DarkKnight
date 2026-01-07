// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/DkLoadingScreenSettings.h"

TSubclassOf<UUserWidget> UDkLoadingScreenSettings::GetLoadingScreenWidgetClassChecked() const
{
	checkf(!SoftLoadingScreenWidgetClass.IsNull(), TEXT("忘记设置加载界面的WidgetClass了！"));

	// 如果该资源尚未进入内存，则阻塞当前线程把它同步加载进来并返回；若已存在则直接返回指针 (同步加载)
	TSubclassOf<UUserWidget> LoadedLoadingScreenWidget = SoftLoadingScreenWidgetClass.LoadSynchronous();

	return LoadedLoadingScreenWidget;
}
