// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp/DkPickUpActorBase.h"

#include "Components/DkItemComponent.h"
#include "Net/UnrealNetwork.h"
#include "Subsytems/EngineSubsystems/DkInventorySubsystem.h"


ADkPickUpActorBase::ADkPickUpActorBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	ItemComponent = CreateDefaultSubobject<UDkItemComponent>(TEXT("DkItemComponent"));
}

void ADkPickUpActorBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemId);
	DOREPLIFETIME(ThisClass, ItemStack);
}

void ADkPickUpActorBase::SetPickUpItemInfo(const FDkItemInfo& PickUpItemInfo, int32 InItemStack)
{
	ItemComponent->InitializeItemComponent(PickUpItemInfo, InItemStack);
}

void ADkPickUpActorBase::Server_SetItemId_Implementation(int InItemId)
{
	ItemId = InItemId;
	UpdatePickUpItemInfo();
}

void ADkPickUpActorBase::Server_SetItemStack_Implementation(int InStackCount)
{
	if (ItemStack != InStackCount)
	{
		ItemStack = InStackCount;
		UpdatePickUpItemInfo();
	}
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
		SetPickUpItemInfo(*CollectionInfo, ItemStack);
	}
}

void ADkPickUpActorBase::UpdatePickUpItemInfo()
{
	TMap<int, FDkItemInfo> ItemTable = UDkInventorySubsystem::Get()->GetCachedItemTable();
	if (ItemTable.Contains(ItemId))
	{
		FDkItemInfo ItemInfo = ItemTable.FindRef(ItemId);
		SetPickUpItemInfo(ItemInfo, ItemStack);
	}
}

void ADkPickUpActorBase::OnRep_ItemId()
{
	// 客户端收到 ItemID 后，本地查表设置 Mesh 和 ItemComp
	UpdatePickUpItemInfo();
}

void ADkPickUpActorBase::OnRep_ItemStack()
{
	UpdatePickUpItemInfo();
}
