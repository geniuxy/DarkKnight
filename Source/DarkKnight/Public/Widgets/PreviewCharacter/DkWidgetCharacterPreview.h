// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DkWidgetCharacterPreview.generated.h"

class UImage;
class ADkPreviewActorBase;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkWidgetCharacterPreview : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	//~Begin UUserWidget
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void BeginDestroy() override;
	//~End UUserWidget

	void SpawnRenderActor();
	void ConfigureRenderActor();

private:
	void BeginRenderCapture();
	void UpdateRender();
	void StopRenderCapture();
	
	UPROPERTY(meta=(BindWidget))
	UImage* DisplayImage;
	
	bool bIsDragging = false;
	TWeakObjectPtr<USkeletalMeshComponent> Mesh;

	FVector2D CurrentMousePosition;
	FVector2D LastMousePosition;

	UPROPERTY()
	ADkPreviewActorBase* PreviewActor;

	UPROPERTY(EditDefaultsOnly, Category="Render Actor")
	TSubclassOf<ADkPreviewActorBase> PreviewActorClass;

	UPROPERTY()
	UTextureRenderTarget2D* RenderTarget;

	UPROPERTY(EditDefaultsOnly, Category="Render Actor")
	FName DisplayImageRenderTargetParamName = "RenderTarget";

	UPROPERTY(EditDefaultsOnly, Category="Render Actor")
	int FrameRate = 24;

	float RenderTickInterval;
	FTimerHandle RenderTimerHandle;
};
