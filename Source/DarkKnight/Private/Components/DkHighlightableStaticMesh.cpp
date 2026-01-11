// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkHighlightableStaticMesh.h"

void UDkHighlightableStaticMesh::Highlight_Implementation()
{
	SetOverlayMaterial(HighlightMaterial);
}

void UDkHighlightableStaticMesh::UnHighlight_Implementation()
{
	SetOverlayMaterial(nullptr);
}
