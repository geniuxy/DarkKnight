// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/DkEquippedActorStatic.h"


ADkEquippedActorStatic::ADkEquippedActorStatic()
{
	EquipmentItemStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquipmentItemStaticMesh"));
	EquipmentItemStaticMesh->SetupAttachment(GetRootComponent());
}

void ADkEquippedActorStatic::SetEquipmentStaticMesh(UStaticMesh* InEquipmentItemMesh) const
{
	if (!IsValid(InEquipmentItemMesh)) return;
	EquipmentItemStaticMesh->SetStaticMesh(InEquipmentItemMesh);
}

