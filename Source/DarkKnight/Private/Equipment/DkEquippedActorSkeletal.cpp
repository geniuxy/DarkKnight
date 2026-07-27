// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/DkEquippedActorSkeletal.h"

#include "Subsytems/EngineSubsystems/DkInventorySubsystem.h"


ADkEquippedActorSkeletal::ADkEquippedActorSkeletal()
{
	EquipmentItemSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipmentItemSkeletalMesh"));
	EquipmentItemSkeletalMesh->SetupAttachment(GetRootComponent());
	EquipmentItemSkeletalMesh->SetIsReplicated(true);
}

void ADkEquippedActorSkeletal::SetEquipmentSkeletalMesh(USkeletalMesh* InEquipmentItemMesh) const
{
	if (!IsValid(InEquipmentItemMesh)) return;
	EquipmentItemSkeletalMesh->SetSkeletalMeshAsset(InEquipmentItemMesh);
}

void ADkEquippedActorSkeletal::UpdateEquipmentItemInfo()
{
	Super::UpdateEquipmentItemInfo();

	TMap<int, FDkItemInfo> ItemTable = UDkInventorySubsystem::Get()->GetCachedItemTable();
	if (!ItemTable.Contains(ItemId)) return;

	FDkItemInfo EquipmentInfo = ItemTable.FindRef(ItemId);
	if (!EquipmentInfo.bStaticMesh && EquipmentInfo.ItemSkeletalMesh.IsValid())
	{
		SetEquipmentSkeletalMesh(EquipmentInfo.ItemSkeletalMesh.LoadSynchronous());
	}
}
