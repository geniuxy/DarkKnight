// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp/DkPickUpActorBase.h"

#include "Components/DkHighlightableSkeletalMesh.h"
#include "Components/DkItemComponent.h"
#include "Subsytems/DkInventorySubsystem.h"


ADkPickUpActorBase::ADkPickUpActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemComponent = CreateDefaultSubobject<UDkItemComponent>(TEXT("DkItemComponent"));
}

void ADkPickUpActorBase::SetPickUpItemInfo(const FDkItemInfo* PickUpItemInfo, int32 InItemStack)
{
	ItemComponent->InitializeItemComponent(PickUpItemInfo, InItemStack);
}



