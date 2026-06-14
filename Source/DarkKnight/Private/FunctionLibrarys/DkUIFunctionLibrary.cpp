// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrarys/DkUIFunctionLibrary.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"
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
	case EDkInputMode::GameAndUI:
		// TODO: Crunch里有GameAndUI相关的代码，需要时可以参考，主要用于游戏视角和界面UI并存时的情况
		break;
	}
}

bool UDkUIFunctionLibrary::PositionWidgetAtMouse(UUserWidget* Widget, FVector2D Offset, bool bAutoFlipX,
                                                 bool bAutoFlipY, float Margin)
{
	if (!Widget || !Widget->IsInViewport()) return false;

	// 获取视口和鼠标
	UWorld* World = Widget->GetWorld();
	if (!World) return false;

	FVector2D MousePos;
	if (UGameViewportClient* VP = World->GetGameViewport())
	{
		VP->GetMousePosition(MousePos); // 系统硬件像素（受 DPI 缩放）
	}

	// FVector2D WidgetSize = FVector2D::ZeroVector;
	// if (TSharedPtr<SWidget> SlateWidget = Widget->TakeWidget())
	// {
	// 	SlateWidget->SlatePrepass(FSlateApplication::Get().GetApplicationScale());
	//
	// 	FVector2D Desired = SlateWidget->GetDesiredSize();
	// 	float DPIScale = World->GetGameViewport()->GetDPIScale();
	// 	FVector2D Actual = Widget->GetCachedGeometry().GetAbsoluteSize();
	// 	float AppScale = FSlateApplication::Get().GetApplicationScale();
	//
	// 	Debug::Print(FString::Printf(TEXT("Desired: %s, DPI: %.2f, AppScale: %.2f, AbsoluteSize: %s, Ratio: %.2f"),
	// 		*Desired.ToString(), DPIScale, AppScale, *Actual.ToString(), 
	// 		Actual.X / (Desired.X * DPIScale)));
	// 	// WidgetSize = Desired * DPIScale * 1.25f; // TODO: 这个1.25是哪来的？如何造成的？
	// 	WidgetSize = Actual;
	// }
	// 这种算法在tick时，会得到WidgetSize为0,0，从而瞬闪
	const FVector2D WidgetSize = Widget->GetCachedGeometry().GetAbsoluteSize();
	// Debug::Print(FString::Printf(TEXT("宽度：%f,高度：%f"), WidgetSize.X, WidgetSize.Y));
	FVector2D ViewportSize = GetViewportSizeInPixels(World);

	// 计算初始位置（默认右下）
	FVector2D FinalPos = MousePos + Offset;

	if (bAutoFlipX && FinalPos.X + WidgetSize.X > ViewportSize.X - Margin)
	{
		FinalPos.X = MousePos.X - WidgetSize.X - Offset.X;
	}
	if (bAutoFlipY && FinalPos.Y + WidgetSize.Y > ViewportSize.Y - Margin)
	{
		FinalPos.Y = MousePos.Y - WidgetSize.Y - Offset.Y;
	}

	// 最终钳制
	FinalPos.X = FMath::Clamp(FinalPos.X, 0, ViewportSize.X - WidgetSize.X);
	FinalPos.Y = FMath::Clamp(FinalPos.Y, 0, ViewportSize.Y - WidgetSize.Y);

	Widget->SetPositionInViewport(FinalPos);
	return true;
}

FVector2D UDkUIFunctionLibrary::GetViewportSizeInPixels(const UObject* WorldContextObject)
{
	if (!GEngine) return FVector2D::ZeroVector;

	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,
	                                                             EGetWorldErrorMode::LogAndReturnNull))
	{
		if (const UGameViewportClient* VP = World->GetGameViewport())
		{
			FVector2D Size;
			VP->GetViewportSize(Size);
			return Size;
		}
	}
	return FVector2D::ZeroVector;
}

FVector2D UDkUIFunctionLibrary::GetWidgetPosition(UWidget* Widget)
{
	const FGeometry Geometry = Widget->GetCachedGeometry();
	FVector2D PixelPosition; // 物理像素, 操作系统认的硬件像素
	FVector2D ViewportPosition; // 逻辑/虚拟像素, 数值不受 DPI 缩放影响
	USlateBlueprintLibrary::LocalToViewport(
		Widget, Geometry, USlateBlueprintLibrary::GetLocalTopLeft(Geometry), PixelPosition, ViewportPosition
	);
	return ViewportPosition;
}

FVector2D UDkUIFunctionLibrary::GetWidgetSize(UWidget* Widget)
{
	const FGeometry Geometry = Widget->GetCachedGeometry();
	return Geometry.GetLocalSize();
}

bool UDkUIFunctionLibrary::IsWithInBounds(
	const FVector2D& BoundaryPos, const FVector2D& WidgetSize, const FVector2D& MousePos)
{
	return MousePos.X >= BoundaryPos.X && MousePos.X <= (BoundaryPos.X + WidgetSize.X) &&
		MousePos.Y >= BoundaryPos.Y && MousePos.Y <= (BoundaryPos.Y + WidgetSize.Y);
}

FVector2D UDkUIFunctionLibrary::GetRelativeWidgetPosition(
	const FVector2D& BoundaryLeftTopPos,	// 这是相对于ViewPort的Boundary左上角的位置(Local Space)
	const FVector2D& BoundarySize,			// 这是相对于ViewPort的Boundary的Local Size
	const FVector2D& WidgetSize,			// 这是相对于ViewPort的Widget(比如ItemDescription)的LocalSize
	const FVector2D& MousePos,				// 这是相对于ViewPort的鼠标的位置(Local Space)
	const FVector2D& Margin)				// 这个是与其他控件的间距
{
	FVector2D RelativePos =
		FVector2D(MousePos.X - Margin.X - WidgetSize.X * 0.6f - BoundaryLeftTopPos.X,
		          MousePos.Y + Margin.Y - BoundaryLeftTopPos.Y
		);

	if (RelativePos.X + WidgetSize.X > BoundarySize.X)
	{
		RelativePos.X = BoundarySize.X - WidgetSize.X;
	}
	if (RelativePos.X < 0.f)
	{
		RelativePos.X = 0.f;
	}

	if (RelativePos.Y + WidgetSize.Y > BoundarySize.Y)
	{
		RelativePos.Y = MousePos.Y - BoundaryLeftTopPos.Y - Margin.Y - WidgetSize.Y;
	}
	if (RelativePos.Y < 0.f)
	{
		RelativePos.Y = 0.f;
	}

	return RelativePos;
}
