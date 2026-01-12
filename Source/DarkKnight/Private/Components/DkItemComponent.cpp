// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkItemComponent.h"

UDkItemComponent::UDkItemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	PickUpMessage = TEXT("F - 捡起");
}
