// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/DkEquippedActorSkeletal.h"


ADkEquippedActorSkeletal::ADkEquippedActorSkeletal()
{
	EquipmentItemSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipmentItemSkeletalMesh"));
	EquipmentItemSkeletalMesh->SetupAttachment(GetRootComponent());
}

void ADkEquippedActorSkeletal::SetEquipmentSkeletalMesh(USkeletalMesh* InEquipmentItemMesh) const
{
	if (!IsValid(InEquipmentItemMesh)) return;
	EquipmentItemSkeletalMesh->SetSkeletalMeshAsset(InEquipmentItemMesh);
}

