// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Interfaces/HighlightInterface.h"
#include "DkHighlightableStaticMesh.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class DARKKNIGHT_API UDkHighlightableStaticMesh : public UStaticMeshComponent, public IHighlightInterface
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
