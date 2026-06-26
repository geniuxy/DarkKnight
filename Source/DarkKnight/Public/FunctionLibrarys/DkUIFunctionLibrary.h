// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "DkTypes/DkEnums.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DkUIFunctionLibrary.generated.h"

class UWidget;
class UDkWidgetActivatableBase;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category="UI Function Library")
	static TSoftClassPtr<UDkWidgetActivatableBase> GetUISoftWidgetClassByTag(
		UPARAM(meta=(Categories="Dk.Widget")) FGameplayTag InWidgetTag
	);

	UFUNCTION(BlueprintPure, Category="UI Function Library")
	static TSoftObjectPtr<UTexture2D> GetSoftImageByTag(UPARAM(meta=(Categories="Dk.Image")) FGameplayTag InImageTag);

	UFUNCTION(BlueprintCallable, Category="UI Function Library", meta=(WorldContext = "WorldContextObject"))
	static void ToggleInputMode(const UObject* WorldContextObject, EDkInputMode InInputMode);

	UFUNCTION(BlueprintCallable, Category="UI Function Library")
	static EWidgetStackType GetWidgetStackTypeByTag(UPARAM(meta=(Categories="Dk.WidgetStack")) FGameplayTag InWidgetStackTag);

	UFUNCTION(BlueprintCallable, Category="UI Function Library")
	static FGameplayTag GetWidgetStackTagByType(EWidgetStackType InWidgetStackType);

	/**
	 * 直接定位任意 UserWidget 到鼠标附近（一站式方案）
	 * 自动获取 Widget 尺寸并应用 DPI 转换
	 * 
	 * @param Widget 要定位的控件（必须已在视口中）
	 * @param bAutoFlipX 水平空间不足时是否自动翻转到另一侧
	 * @param bAutoFlipY 垂直空间不足时是否自动翻转到另一侧
	 * @return 是否定位成功
	 */
	UFUNCTION(BlueprintCallable, Category="UI|Layout", meta=(UnsafeDuringActorConstruction="true"))
	static bool PositionWidgetAtMouse(
		UUserWidget* Widget,
		FVector2D Offset = FVector2D(4.0f, 4.0f),
		bool bAutoFlipX = true,
		bool bAutoFlipY = true,
		float Margin = 1.0f
	);

	/**
	 * 获取当前游戏视口的硬件像素尺寸
	 */
	UFUNCTION(BlueprintPure, Category="UI|Layout", meta=(WorldContext="WorldContextObject"))
	static FVector2D GetViewportSizeInPixels(const UObject* WorldContextObject);

	/**
	 * 获取当前Widget在Viewport中的位置
	 */
	UFUNCTION(BlueprintPure, Category="UI|Layout")
	static FVector2D GetWidgetPosition(UWidget* Widget);

	UFUNCTION(BlueprintPure, Category="UI|Layout")
	static FVector2D GetWidgetSize(UWidget* Widget);

	UFUNCTION(BlueprintPure, Category="UI|Layout")
	static bool IsWithInBounds(const FVector2D& BoundaryPos, const FVector2D& WidgetSize, const FVector2D& MousePos);

	UFUNCTION(BlueprintPure, Category="UI|Layout")
	static FVector2D GetRelativeWidgetPosition(
		const FVector2D& BoundaryLeftTopPos,
		const FVector2D& BoundarySize,
		const FVector2D& WidgetSize,
		const FVector2D& MousePos,
		const FVector2D& Margin = FVector2D(4.f, 4.f)
	);
};
