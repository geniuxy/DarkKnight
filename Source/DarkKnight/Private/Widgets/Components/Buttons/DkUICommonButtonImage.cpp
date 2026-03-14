// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/Buttons/DkUICommonButtonImage.h"

#include "CommonLazyImage.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void UDkUICommonButtonImage::SetButtonDisplayImage(const FSlateBrush& InBrush)
{
	if (CommonLazyImage_ButtonImage)
	{
		CommonLazyImage_ButtonImage->SetBrush(InBrush);
	}
}

void UDkUICommonButtonImage::ToggleHighlightState(bool bShouldHighlight)
{
	if (!CommonLazyImage_ButtonImage) return;
	
	if (bShouldHighlight)
	{
		CommonLazyImage_ButtonImage->SetColorAndOpacity(HighlightButtonImageColor);
	}
	else
	{
		CommonLazyImage_ButtonImage->SetColorAndOpacity(DefaultButtonImageColor);
	}
}

void UDkUICommonButtonImage::NativePreConstruct()
{
	Super::NativePreConstruct();

	if (CommonLazyImage_ButtonImage)
	{
		CommonLazyImage_ButtonImage->SetBrushFromLazyTexture(SoftButtonImage);
		CommonLazyImage_ButtonImage->SetDesiredSizeOverride(FVector2D(ButtonImageSize));

		if (IsDesignTime() && UKismetSystemLibrary::IsValidSoftObjectReference(PreviewSoftButtonImage))
		{
			CommonLazyImage_ButtonImage->SetBrushFromLazyTexture(PreviewSoftButtonImage);
		}
	}
}

void UDkUICommonButtonImage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (CommonLazyImage_ButtonImage)
	{
		CommonLazyImage_ButtonImage->SetColorAndOpacity(DefaultButtonImageColor);
	}
}

void UDkUICommonButtonImage::NativeOnHovered()
{
	Super::NativeOnHovered();

	if (bShouldToggleStateWhenHovered)
	{
		ToggleHighlightState(true);
	}
}

void UDkUICommonButtonImage::NativeOnUnhovered()
{
	Super::NativeOnUnhovered();

	if (bShouldToggleStateWhenHovered)
	{
		ToggleHighlightState(false);
	}
}
