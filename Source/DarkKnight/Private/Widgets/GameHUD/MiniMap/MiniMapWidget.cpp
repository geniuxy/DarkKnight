// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameHUD/MiniMap/MiniMapWidget.h"

#include "CommonLazyImage.h"
#include "Characters/DkCharacterBase.h"
#include "Engine/Canvas.h"
#include "FunctionLibrarys/DkAbilitySystemFunctionLibrary.h"
#include "FunctionLibrarys/DkGameFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"

void UMiniMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OwnerCharacter = Cast<ADkCharacterBase>(GetOwningPlayerPawn());

	float MapScaleDownSize = MiniMapImage->GetDynamicMaterial()->K2_GetScalarParameterValue("MapScaleDownSize");
	float BorderCircleSize = MiniMapImage->GetDynamicMaterial()->K2_GetScalarParameterValue("BorderCircleSize");
	MiniMapBorderLength = (RightUpWorldLocation.X - LeftDownWorldLocation.X) / 2.f * MapScaleDownSize;
	ActorCheckDistance = MiniMapBorderLength * BorderCircleSize;
}

void UMiniMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (OwnerCharacter && OwnerCharacter->GetMesh())
	{
		PlayerIcon->SetRenderTransformAngle(OwnerCharacter->GetMesh()->GetComponentRotation().Yaw);
	}

	UpdatePlayerPositionInMiniMap();
	UpdateActorIconsInMiniMap();
}

void UMiniMapWidget::UpdatePlayerPositionInMiniMap()
{
	if (!OwnerCharacter) return;

	double curWorldPosX = OwnerCharacter->GetActorLocation().X;
	double curPosXInMiniMap = UKismetMathLibrary::MapRangeUnclamped(
		curWorldPosX, LeftDownWorldLocation.X, RightUpWorldLocation.X, 0, 1
	);
	MiniMapImage->GetDynamicMaterial()->SetScalarParameterValue("PositionX", curPosXInMiniMap);

	double curWorldPosY = OwnerCharacter->GetActorLocation().Y;
	double curPosYInMiniMap = UKismetMathLibrary::MapRangeUnclamped(
		curWorldPosY, LeftDownWorldLocation.Y, RightUpWorldLocation.Y, 0, 1
	);
	MiniMapImage->GetDynamicMaterial()->SetScalarParameterValue("PositionY", curPosYInMiniMap);
}

void UMiniMapWidget::UpdateActorIconsInMiniMap()
{
	if (!OwnerCharacter) return;

	UTextureRenderTarget2D* CachedActorsRenderTargetTexture = ActorsRenderTargetTexture.LoadSynchronous();
	if (!CachedActorsRenderTargetTexture) return;
	UKismetRenderingLibrary::ClearRenderTarget2D(OwnerCharacter, CachedActorsRenderTargetTexture);

	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(
		OwnerCharacter, CachedActorsRenderTargetTexture, CachedCanvas, CachedCanvasSize, CachedContext
	);

	TArray<ADkCharacterBase*> ActorsInRange = UDkGameFunctionLibrary::GetUnitsInRange<ADkCharacterBase>(
		OwnerCharacter, OwnerCharacter->GetActorLocation(), ActorCheckDistance, 2
	);

	DrawActorIconsToMiniMap(ActorsInRange);

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, CachedContext);
}

void UMiniMapWidget::DrawActorIconsToMiniMap(TArray<ADkCharacterBase*> Actors, UTexture2D* DrawTexture)
{
	if (Actors.IsEmpty()) return;
	if (!CachedCanvas) return;

	for (ADkCharacterBase* Actor : Actors)
	{
		if (UDkAbilitySystemFunctionLibrary::IsActorDead(Actor)) continue;
		if (DrawTexture)
		{
			CachedCanvas->K2_DrawTexture(
				DrawTexture, ConvertWorldLocationToMiniMap(Actor->GetActorLocation()) - FVector2D(IconSize * 4),
				FVector2D(IconSize * 8), FVector2D::ZeroVector
			);
		}
		else
		{
			CachedCanvas->K2_DrawPolygon(
				DrawTexture, ConvertWorldLocationToMiniMap(Actor->GetActorLocation()),
				FVector2D(IconSize * 2), 20, FLinearColor(0.8f, 0.f, 0.f, 1.f)
			);
		}
	}
}

FVector2D UMiniMapWidget::ConvertWorldLocationToMiniMap(FVector InWorldLocation)
{
	if (!OwnerCharacter || CachedCanvasSize.X <= 0.f || CachedCanvasSize.Y <= 0.f)
		return FVector2D::ZeroVector;

	const FVector OwnerLocation = OwnerCharacter->GetActorLocation();

	// 以主角为中心的视野边界（世界坐标）
	const float Left = OwnerLocation.X - MiniMapBorderLength;
	const float Right = OwnerLocation.X + MiniMapBorderLength;
	const float Bottom = OwnerLocation.Y - MiniMapBorderLength;
	const float Top = OwnerLocation.Y + MiniMapBorderLength;

	const double MapX = UKismetMathLibrary::MapRangeUnclamped(
		InWorldLocation.X, Left, Right, 0.0f, CachedCanvasSize.X
	);

	const double MapY = UKismetMathLibrary::MapRangeUnclamped(
		InWorldLocation.Y, Bottom, Top, 0.0f, CachedCanvasSize.Y
	);

	return FVector2D(MapX, MapY);
}
