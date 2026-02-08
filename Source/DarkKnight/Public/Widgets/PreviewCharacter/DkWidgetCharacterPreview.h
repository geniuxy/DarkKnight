// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DkWidgetCharacterPreview.generated.h"

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
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	//~End UUserWidget

private:
	bool bIsDragging = false;
	TWeakObjectPtr<USkeletalMeshComponent> Mesh;

	FVector2D CurrentMousePosition;
	FVector2D LastMousePosition;
};
