// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp/DkPickUpActorBase.h"

#include "Components/DkItemComponent.h"
#include "Subsytems/EngineSubsystems/DkInventorySubsystem.h"


ADkPickUpActorBase::ADkPickUpActorBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	ItemComponent = CreateDefaultSubobject<UDkItemComponent>(TEXT("DkItemComponent"));
}

void ADkPickUpActorBase::SetPickUpItemInfo(const FDkItemInfo* PickUpItemInfo, int32 InItemStack)
{
	ItemComponent->InitializeItemComponent(PickUpItemInfo, InItemStack);
}

void ADkPickUpActorBase::BeginPlay()
{
	Super::BeginPlay();

	if (IsCollectable)
	{
		FTimerHandle InitCollectionTimeHandle;
		GetWorldTimerManager().SetTimer(InitCollectionTimeHandle, this, &ThisClass::InitCollectionsInfo, 0.2f);
	}
}

void ADkPickUpActorBase::InitCollectionsInfo()
{
	if (const FDkItemInfo* CollectionInfo =
		UDkInventorySubsystem::Get()->GetCachedItemTable().Find(ItemComponent->GetItemId()))
	{
		SetPickUpItemInfo(CollectionInfo, StackCount);
	}
}
