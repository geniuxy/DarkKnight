// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/DkEquippedActorBase.h"


ADkEquippedActorBase::ADkEquippedActorBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	EquipmentItemSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipmentItemSkeletalMesh"));
	EquipmentItemSkeletalMesh->SetupAttachment(GetRootComponent());

	EquipmentItemStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquipmentItemStaticMesh"));
	EquipmentItemStaticMesh->SetupAttachment(EquipmentItemSkeletalMesh);
}

void ADkEquippedActorBase::SetEquipmentSkeletalMesh(USkeletalMesh* InEquipmentItemMesh) const
{
	EquipmentItemSkeletalMesh->SetSkeletalMeshAsset(InEquipmentItemMesh);
}

void ADkEquippedActorBase::SetEquipmentStaticMesh(UStaticMesh* InEquipmentItemMesh) const
{
	EquipmentItemStaticMesh->SetStaticMesh(InEquipmentItemMesh);
}


