// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/DkCharacterMerchant.h"

#include "Components/InventoryComps/DkNpcInventoryComp.h"


ADkCharacterMerchant::ADkCharacterMerchant()
{
	PrimaryActorTick.bCanEverTick = false;

	MerchantInventoryComp = CreateDefaultSubobject<UDkNpcInventoryComp>(TEXT("NpcInventoryComp"));
}

void ADkCharacterMerchant::BeginPlay()
{
	Super::BeginPlay();
}

