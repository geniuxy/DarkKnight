// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkPlayerDialogComponent.h"


UDkPlayerDialogComponent::UDkPlayerDialogComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CachedDialogTags.Reset();
}

bool UDkPlayerDialogComponent::FindDialogGameplayTag(FGameplayTag InTag)
{
	return CachedDialogTags.HasTagExact(InTag);
}

