// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/DkEquippedActorStaticWithAdditional.h"

ADkEquippedActorStaticWithAdditional::ADkEquippedActorStaticWithAdditional()
{
	AdditionalMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AdditionalMesh"));
	AdditionalMesh->SetupAttachment(EquipmentItemStaticMesh);
}
