// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Composites/DkInventoryLeafImage.h"

#include "CommonLazyImage.h"
#include "Components/SizeBox.h"

void UDkInventoryLeafImage::SetImage(UTexture2D* Texture) const
{
	Image_Icon->SetBrushFromTexture(Texture);
}

void UDkInventoryLeafImage::SetBoxSize(const FVector2D& Size) const
{
	SizeBox_Icon->SetWidthOverride(Size.X);
	SizeBox_Icon->SetHeightOverride(Size.Y);
}

void UDkInventoryLeafImage::SetImageSize(const FVector2D& Size) const
{
	Image_Icon->SetDesiredSizeOverride(Size);
}

FVector2D UDkInventoryLeafImage::GetImageSize() const
{
	return Image_Icon->GetDesiredSize();
}
