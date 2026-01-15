// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrarys/DkUIFunctionLibrary.h"

#include "Settings/DkUIDeveloperSettings.h"
#include "Slate/SceneViewport.h"

TSoftClassPtr<UDkWidgetActivatableBase> UDkUIFunctionLibrary::GetUISoftWidgetClassByTag(FGameplayTag InWidgetTag)
{
	const UDkUIDeveloperSettings* UIDeveloperSettings = GetDefault<UDkUIDeveloperSettings>();

	checkf(
		UIDeveloperSettings->UIWidgetMap.Contains(InWidgetTag),
		TEXT("找不到对应的Widget(Tag为%s)"),
		*InWidgetTag.ToString()
	);

	return UIDeveloperSettings->UIWidgetMap.FindRef(InWidgetTag);
}

TSoftObjectPtr<UTexture2D> UDkUIFunctionLibrary::GetSoftImageByTag(FGameplayTag InImageTag)
{
	const UDkUIDeveloperSettings* UIDeveloperSettings = GetDefault<UDkUIDeveloperSettings>();

	checkf(
		UIDeveloperSettings->ImageMap.Contains(InImageTag),
		TEXT("找不到对应的Image(Tag为%s)"),
		*InImageTag.ToString()
	);

	return UIDeveloperSettings->ImageMap.FindRef(InImageTag);
}

void UDkUIFunctionLibrary::ToggleInputMode(const UObject* WorldContextObject, EDkInputMode InInputMode)
{
	APlayerController* PlayerController = nullptr;
	if (GEngine)
	{
		if (UWorld* World =
			GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
		{
			PlayerController = World->GetFirstPlayerController();
		}
	}
	if (!PlayerController || !PlayerController->IsLocalController()) return;

	FInputModeGameOnly GameOnlyInputMode;
	FInputModeUIOnly UIOnlyInputMode;
	UIOnlyInputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);

	switch (InInputMode)
	{
	case EDkInputMode::GameOnly:
		PlayerController->SetInputMode(GameOnlyInputMode);
		PlayerController->bShowMouseCursor = false;
		break;
	case EDkInputMode::UIOnly:
		PlayerController->SetInputMode(UIOnlyInputMode);
		PlayerController->bShowMouseCursor = true;

		// 将鼠标定位到显示屏中心
		if (GEngine && GEngine->GameViewport)
		{
			if (FSceneViewport* SceneViewport = GEngine->GameViewport->GetGameViewport())
			{
				// 1. 拿到视口在桌面上的绝对几何
				FGeometry ViewportGeometry = SceneViewport->GetCachedGeometry();
				// 2. 计算中心点的绝对屏幕坐标
				FVector2D CenterLocal(ViewportGeometry.GetLocalSize() * 0.5f);
				FVector2D CenterScreen = ViewportGeometry.LocalToAbsolute(CenterLocal);
				// 3. 设置系统光标位置
				FSlateApplication::Get().SetCursorPos(CenterScreen);
			}
		}
		
		break;
	}

}
