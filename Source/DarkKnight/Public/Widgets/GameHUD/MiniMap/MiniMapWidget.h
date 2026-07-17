// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "MiniMapWidget.generated.h"

class ADkPickUpActorBase;
class UBorder;
class ADkPlayerStateBase;
class ADkCharacterBase;
class UCommonTextBlock;
class UCommonLazyImage;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UMiniMapWidget : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonLazyImage> MiniMapImage;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonLazyImage> PlayerIcon;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> DistanceText;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBorder> TaskTrackingBorder;

	UPROPERTY()
	ACharacter* OwnerCharacter;

	UPROPERTY()
	ADkPlayerStateBase* OwnerPlayerState;
	
	UPROPERTY(EditDefaultsOnly, Category="Render Target")
	TSoftObjectPtr<UTextureRenderTarget2D> ActorsRenderTargetTexture;
	
	UPROPERTY(EditDefaultsOnly, Category="Render Target")
	float IconSize = 80.f;

	UPROPERTY(EditDefaultsOnly, Category="Render Target")
	float PointSize = 20.f;

	UPROPERTY(EditDefaultsOnly, Category="Mini Map Details")
	FVector2D LeftDownWorldLocation = {-4000.f, -4000.f};

	UPROPERTY(EditDefaultsOnly, Category="Mini Map Details")
	FVector2D RightUpWorldLocation = {4000.f, 4000.f};

	UPROPERTY()
	float MiniMapBorderLength;
	UPROPERTY()
	float ActorCheckDistance;
	UPROPERTY()
	UCanvas* CachedCanvas;
	UPROPERTY()
	FVector2D CachedCanvasSize;
	UPROPERTY()
	FDrawToRenderTargetContext CachedContext;
	
	void UpdatePlayerPositionInMiniMap();

	void DrawElements();
	void UpdateActorIconsInMiniMap();
	void UpdateCollectionIconsInMiniMap();
	void DrawTaskTrackingLine();

	void DrawActorIconsToMiniMap(TArray<ADkCharacterBase*> Actors);
	void DrawCollectionIconsToMiniMap(TArray<ADkPickUpActorBase*> Actors);
	FVector2D ConvertWorldLocationToMiniMap(FVector InWorldLocation);
};
