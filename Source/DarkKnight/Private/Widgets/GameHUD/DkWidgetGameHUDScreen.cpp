// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameHUD/DkWidgetGameHUDScreen.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "GAS/DkAttributeSet.h"
#include "Widgets/GameHUD/Stats/ValueGauge.h"
#include "Widgets/GameHUD/TaskNotice/TaskNoticeWidget.h"

void UDkWidgetGameHUDScreen::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	bSupportsActivationFocus = false; // 该设置可使Widget不会阻止鼠标等输入
}

void UDkWidgetGameHUDScreen::NativeOnActivated()
{
	Super::NativeOnActivated();

	UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::GameOnly);

	TaskNoticeWidget->SetRenderOpacity(0.f);

	OwnerAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPlayerPawn());
	if (OwnerAbilitySystemComponent)
	{
		HealthBar->SetAndBoundToGameplayAttribute(
			OwnerAbilitySystemComponent,
			UDkAttributeSet::GetHealthAttribute(),
			UDkAttributeSet::GetMaxHealthAttribute()
		);

		EnergyBar->SetAndBoundToGameplayAttribute(
			OwnerAbilitySystemComponent,
			UDkAttributeSet::GetEnergyAttribute(),
			UDkAttributeSet::GetMaxEnergyAttribute()
		);
	}
}
