// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameHUD/MiniMap/MiniMapWidget.h"

#include "CommonLazyImage.h"
#include "Characters/DkCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"

void UMiniMapWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OwnerCharacter = Cast<ADkCharacterBase>(GetOwningPlayerPawn());
}

void UMiniMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (OwnerCharacter && OwnerCharacter->GetMesh())
	{
		PlayerIcon->SetRenderTransformAngle(OwnerCharacter->GetMesh()->GetComponentRotation().Yaw);
	}

	UpdateMiniMap();
}

void UMiniMapWidget::UpdateMiniMap()
{
	if (!OwnerCharacter) return;

	double curWorldPosX = OwnerCharacter->GetActorLocation().X;
	double curPosXInMiniMap = UKismetMathLibrary::MapRangeUnclamped(curWorldPosX, -4000, 4000, 0, 1);
	MiniMapImage->GetDynamicMaterial()->SetScalarParameterValue("PositionX", curPosXInMiniMap);

	double curWorldPosY = OwnerCharacter->GetActorLocation().Y;
	double curPosYInMiniMap = UKismetMathLibrary::MapRangeUnclamped(curWorldPosY, -4000, 4000, 0, 1);
	MiniMapImage->GetDynamicMaterial()->SetScalarParameterValue("PositionY", curPosYInMiniMap);
}
