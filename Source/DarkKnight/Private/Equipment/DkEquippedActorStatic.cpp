// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/DkEquippedActorStatic.h"

#include "Subsytems/EngineSubsystems/DkInventorySubsystem.h"


ADkEquippedActorStatic::ADkEquippedActorStatic()
{
	EquipmentItemStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquipmentItemStaticMesh"));
	EquipmentItemStaticMesh->SetupAttachment(GetRootComponent());
	EquipmentItemStaticMesh->SetIsReplicated(true);
}

void ADkEquippedActorStatic::SetEquipmentStaticMesh(UStaticMesh* InEquipmentItemMesh) const
{
	if (!IsValid(InEquipmentItemMesh)) return;
	EquipmentItemStaticMesh->SetStaticMesh(InEquipmentItemMesh);
}

void ADkEquippedActorStatic::UpdateEquipmentItemInfo()
{
	Super::UpdateEquipmentItemInfo();
	
	TMap<int, FDkItemInfo> ItemTable = UDkInventorySubsystem::Get()->GetCachedItemTable();
	if (!ItemTable.Contains(ItemId)) return;
	
	FDkItemInfo EquipmentInfo = ItemTable.FindRef(ItemId);
	if (EquipmentInfo.bStaticMesh && EquipmentInfo.ItemStaticMesh.IsValid())
	{
		SetEquipmentStaticMesh( EquipmentInfo.ItemStaticMesh.LoadSynchronous() );
	}
}

