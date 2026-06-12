// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameHUD/Crosshair/CrosshairLockTargetWidget.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CommonLazyImage.h"
#include "DkGameplayTags.h"

void UCrosshairLockTargetWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CrosshairImage->SetVisibility(ESlateVisibility::Hidden);

	UAbilitySystemComponent* OwnerASC =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPlayerPawn());
	if (OwnerASC)
	{
		OwnerASC->RegisterGameplayTagEvent(DkGameplayTags::Dk_Stats_Crosshair_LockTarget).AddUObject(
			this, &ThisClass::CrosshairTagUpdated
		);
	}
}

void UCrosshairLockTargetWidget::CrosshairTagUpdated(const FGameplayTag Tag, int32 NewCount)
{
	CrosshairImage->SetVisibility(NewCount > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
