// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameHUD/MiniMap/MiniMapWidget.h"

#include "CommonLazyImage.h"
#include "CommonTextBlock.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Characters/DkCharacterBase.h"
#include "Components/Border.h"
#include "Engine/Canvas.h"
#include "FunctionLibrarys/DkAbilitySystemFunctionLibrary.h"
#include "FunctionLibrarys/DkGameFunctionLibrary.h"
#include "Games/PlayerStates/DkPlayerStateBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Subsytems/EngineSubsystems/DkDataSubsystem.h"

void UMiniMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OwnerCharacter = Cast<ACharacter>(GetOwningPlayerPawn());
	if (OwnerCharacter)
	{
		OwnerPlayerState = OwnerCharacter->GetPlayerState<ADkPlayerStateBase>();
	}

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

	DrawElements();
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

void UMiniMapWidget::DrawElements()
{
	UTextureRenderTarget2D* CachedActorsRenderTargetTexture = ActorsRenderTargetTexture.LoadSynchronous();
	if (!CachedActorsRenderTargetTexture) return;
	UKismetRenderingLibrary::ClearRenderTarget2D(OwnerCharacter, CachedActorsRenderTargetTexture);

	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(
		OwnerCharacter, CachedActorsRenderTargetTexture, CachedCanvas, CachedCanvasSize, CachedContext
	);

	UpdateActorIconsInMiniMap();
	DrawTaskTrackingLine();

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(this, CachedContext);
}

void UMiniMapWidget::UpdateActorIconsInMiniMap()
{
	if (!OwnerCharacter) return;

	TArray<ADkCharacterBase*> ActorsInRange = UDkGameFunctionLibrary::GetUnitsInRange<ADkCharacterBase>(
		OwnerCharacter, OwnerCharacter->GetActorLocation(), ActorCheckDistance, 2
	);

	DrawActorIconsToMiniMap(ActorsInRange);

	TArray<ADkCharacterBase*> NpcListInRange = UDkGameFunctionLibrary::GetUnitsInRange<ADkCharacterBase>(
		OwnerCharacter, OwnerCharacter->GetActorLocation(), ActorCheckDistance, 3
	);

	DrawActorIconsToMiniMap(NpcListInRange);
}

void UMiniMapWidget::DrawTaskTrackingLine()
{
	if (!OwnerCharacter || !OwnerPlayerState) return;

	FGameplayTag CurTrackingTaskTag = OwnerPlayerState->GetCurTrackingTaskTag();
	if (!CurTrackingTaskTag.IsValid())
	{
		TaskTrackingBorder->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	AActor* TaskTrackingActor = UDkDataSubsystem::Get()->GetTaskTrackingActor(CurTrackingTaskTag);
	if (!TaskTrackingActor)
	{
		TaskTrackingBorder->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}

	TaskTrackingBorder->SetVisibility(ESlateVisibility::Visible);
	float DistanceToTarget = FMath::Min(OwnerCharacter->GetDistanceTo(TaskTrackingActor) / 100.f, 9999.f);
	if (DistanceToTarget > ActorCheckDistance) return;

	// 这种做法有个缺点是，在空中时距离为0.f
	if (!CachedCanvas) return;
	float DistanceWalkToTarget = 0.f;
	UNavigationPath* OwnerCharacterToTargetPath = UNavigationSystemV1::FindPathToActorSynchronously(
		this, OwnerCharacter->GetActorLocation(), TaskTrackingActor
	);
	if (!OwnerCharacterToTargetPath) return;
	for (int i = 0; i < OwnerCharacterToTargetPath->PathPoints.Num(); ++i)
	{
		if (i == OwnerCharacterToTargetPath->PathPoints.Num() - 1) continue;
		FVector2D PrevPoint = ConvertWorldLocationToMiniMap(OwnerCharacterToTargetPath->PathPoints[i]);
		FVector2D NextPoint = ConvertWorldLocationToMiniMap(OwnerCharacterToTargetPath->PathPoints[i + 1]);
		CachedCanvas->K2_DrawLine(PrevPoint, NextPoint, 5.f);
		DistanceWalkToTarget += FMath::Abs(FVector::Distance(
				OwnerCharacterToTargetPath->PathPoints[i], OwnerCharacterToTargetPath->PathPoints[i + 1])
		);
	}

	FNumberFormattingOptions FormatOps = FNumberFormattingOptions().SetMaximumFractionalDigits(0);
	FormatOps.UseGrouping = false;
	DistanceText->SetText(FText::AsNumber(DistanceWalkToTarget / 100.f, &FormatOps));
}

void UMiniMapWidget::DrawActorIconsToMiniMap(TArray<ADkCharacterBase*> Actors)
{
	if (Actors.IsEmpty()) return;
	if (!CachedCanvas) return;

	for (ADkCharacterBase* Actor : Actors)
	{
		if (UDkAbilitySystemFunctionLibrary::IsActorDead(Actor)) continue;
		if (Actor->GetTaskTrackingIcon())
		{
			CachedCanvas->K2_DrawTexture(
				Actor->GetTaskTrackingIcon(),
				ConvertWorldLocationToMiniMap(Actor->GetActorLocation()) - FVector2D(IconSize / 2.f),
				FVector2D(IconSize),
				FVector2D::ZeroVector,
				FVector2D::UnitVector,
				FLinearColor::White,
				BLEND_Translucent,
				90.f
			);
		}
		else
		{
			CachedCanvas->K2_DrawPolygon(
				nullptr, ConvertWorldLocationToMiniMap(Actor->GetActorLocation()),
				FVector2D(PointSize), 20, FLinearColor(0.8f, 0.f, 0.f, 1.f)
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
