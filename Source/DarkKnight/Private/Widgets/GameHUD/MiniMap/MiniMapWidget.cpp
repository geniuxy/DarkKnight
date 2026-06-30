// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameHUD/MiniMap/MiniMapWidget.h"

#include "CommonLazyImage.h"
#include "Characters/DkCharacterBase.h"

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
}
