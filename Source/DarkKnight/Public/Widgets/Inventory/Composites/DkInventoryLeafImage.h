// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkGameplayTags.h"
#include "DkInventoryLeaf.h"
#include "DkInventoryLeafImage.generated.h"

class UCommonLazyImage;
class USizeBox;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkInventoryLeafImage : public UDkInventoryLeaf
{
	GENERATED_BODY()

public:
	UDkInventoryLeafImage()
	{
		FragmentTag = DkGameplayTags::Dk_Inventory_Fragment_Icon;
	}
	
	void SetImage(UTexture2D* Texture) const;
	void SetBoxSize(const FVector2D& Size) const;
	void SetImageSize(const FVector2D& Size) const;
	FVector2D GetImageSize() const;

private:
	//***** Bound Widgets *****//
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox_Icon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCommonLazyImage> Image_Icon;
	//***** Bound Widgets *****//
};
