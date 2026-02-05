// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkHighlightableSkeletalMesh.h"

void UDkHighlightableSkeletalMesh::Highlight_Implementation()
{
	SetOverlayMaterial(HighlightMaterial);
}

void UDkHighlightableSkeletalMesh::UnHighlight_Implementation()
{
	SetOverlayMaterial(nullptr);
}
