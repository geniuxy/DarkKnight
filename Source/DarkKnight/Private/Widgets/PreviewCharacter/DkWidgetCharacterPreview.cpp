// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/PreviewCharacter/DkWidgetCharacterPreview.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Characters/PreviewActor/DkPreviewActorBase.h"
#include "Components/Image.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Subsytems/EngineSubsystems/DkInventorySubsystem.h"

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
}

void UDkWidgetCharacterPreview::NativeConstruct()
{
	Super::NativeConstruct();

	SpawnRenderActor();
	ConfigureRenderActor();
	BeginRenderCapture();

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

	if (PreviewActor)
	{
		PreviewActor->UpdateRender();
	}
}

void UDkWidgetCharacterPreview::BeginDestroy()
{
	StopRenderCapture();
	Super::BeginDestroy();
}

void UDkWidgetCharacterPreview::SpawnRenderActor()
{
	if (!PreviewActorClass) return;
	if (PreviewActor) return;

	UWorld* World = GetWorld();
	if (!World) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	PreviewActor = World->SpawnActor<ADkPreviewActorBase>(PreviewActorClass, SpawnParams);
}

void UDkWidgetCharacterPreview::ConfigureRenderActor()
{
	if (!IsValid(PreviewActor)) return;
	if (!RenderTarget)
	{
		RenderTarget = NewObject<UTextureRenderTarget2D>(this);
		RenderTarget->InitAutoFormat(512, 512);
		RenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8_SRGB;
		PreviewActor->SetRenderTarget(RenderTarget);
	}

	UMaterialInstanceDynamic* DisplayImageDynamicMaterial = DisplayImage->GetDynamicMaterial();
	if (DisplayImageDynamicMaterial)
	{
		DisplayImageDynamicMaterial->SetTextureParameterValue(DisplayImageRenderTargetParamName, RenderTarget);
	}
}

void UDkWidgetCharacterPreview::BeginRenderCapture()
{
	RenderTickInterval = 1.f / (float)FrameRate;
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(RenderTimerHandle, this, &ThisClass::UpdateRender, RenderTickInterval, true);
	}
}

void UDkWidgetCharacterPreview::UpdateRender()
{
	if (PreviewActor)
	{
		PreviewActor->UpdateRender();
	}
}

void UDkWidgetCharacterPreview::StopRenderCapture()
{
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().ClearTimer(RenderTimerHandle);
	}
}
