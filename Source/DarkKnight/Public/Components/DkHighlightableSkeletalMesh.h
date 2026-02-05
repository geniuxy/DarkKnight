// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Interfaces/HighlightInterface.h"
#include "DkHighlightableSkeletalMesh.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkHighlightableSkeletalMesh : public USkeletalMeshComponent, public IHighlightInterface
{
	GENERATED_BODY()

public:
	//~Begin IHighlightInterface Interface
	virtual void Highlight_Implementation() override;
	virtual void UnHighlight_Implementation() override;
	//~End IHighlightInterface Interface
	
private:

	UPROPERTY(EditAnywhere, Category = "Highlight")
	TObjectPtr<UMaterialInterface> HighlightMaterial;
};
