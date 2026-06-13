// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameHUD/Crosshair/CrosshairLockTargetWidget.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "CommonLazyImage.h"
#include "Components/CanvasPanelSlot.h"
#include "DarkKnightDebugHelper.h"
#include "DkGameplayTags.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "GAS/DkAbilitySystemComponent.h"

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

	CachedPlayerController = GetOwningPlayer();

	CrosshairCanvasPanelSlot = Cast<UCanvasPanelSlot>(Slot);
	if (!CrosshairCanvasPanelSlot)
	{
		Debug::Print(TEXT("CrosshairWidget需要CanvasPanel作为父Widget"));
	}
}

void UCrosshairLockTargetWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (CrosshairImage->GetVisibility() == ESlateVisibility::Visible)
	{
		UpdateCrosshairPosition();
	}
}

void UCrosshairLockTargetWidget::CrosshairTagUpdated(const FGameplayTag Tag, int32 NewCount)
{
	CrosshairImage->SetVisibility(NewCount > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void UCrosshairLockTargetWidget::UpdateCrosshairPosition()
{
	if (!CachedPlayerController || !CrosshairCanvasPanelSlot) return;

	float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(this);
	int32 SizeX, SizeY;
	CachedPlayerController->GetViewportSize(SizeX, SizeY);

	UDkAbilitySystemComponent* OwnerASC = Cast<UDkAbilitySystemComponent>(
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwningPlayerPawn())
	);
	AActor* CurrentAimTarget = OwnerASC->GetLockTarget();
	if (!CurrentAimTarget)
	{
		return;
	}

	FVector2D TargetScreenPosition;
	CachedPlayerController->ProjectWorldLocationToScreen(CurrentAimTarget->GetActorLocation(), TargetScreenPosition);
	if (TargetScreenPosition.X > 0 && TargetScreenPosition.X < SizeX &&
		TargetScreenPosition.Y > 0 && TargetScreenPosition.Y < SizeY)
	{
		CrosshairCanvasPanelSlot->SetPosition(TargetScreenPosition / ViewportScale);
	}
}
