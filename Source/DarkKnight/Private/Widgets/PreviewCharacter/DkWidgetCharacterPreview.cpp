// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PreviewCharacter/DkWidgetCharacterPreview.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Characters/PreviewActor/DkPreviewActorBase.h"
#include "Kismet/GameplayStatics.h"

FReply UDkWidgetCharacterPreview::NativeOnMouseButtonDown(const FGeometry& InGeometry,
                                                          const FPointerEvent& InMouseEvent)
{
	CurrentMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	LastMousePosition = CurrentMousePosition;
	bIsDragging = true;

	return FReply::Handled();
}

FReply UDkWidgetCharacterPreview::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsDragging = false;

	return Super::NativeOnMouseButtonUp(InGeometry, InMouseEvent);
}

void UDkWidgetCharacterPreview::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	bIsDragging = false;

	Super::NativeOnMouseLeave(InMouseEvent);
}

void UDkWidgetCharacterPreview::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, ADkPreviewActorBase::StaticClass(), Actors);

	if (Actors.Num() == 0) return;

	ADkPreviewActorBase* PreviewActor = Cast<ADkPreviewActorBase>(Actors[0]);
	if (!IsValid(PreviewActor)) return;

	Mesh = PreviewActor->GetMesh();
}

void UDkWidgetCharacterPreview::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (!bIsDragging) return;

	LastMousePosition = CurrentMousePosition;
	CurrentMousePosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(this);
	const float HorizontalDelta = LastMousePosition.X - CurrentMousePosition.X;

	if (!Mesh.IsValid()) return;
	Mesh->AddRelativeRotation(FRotator(0.f, HorizontalDelta, 0.f));
}
