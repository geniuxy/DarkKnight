// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DkCharacterHero.h"

#include "Components/DkInventoryComponent.h"


ADkCharacterHero::ADkCharacterHero()
{
	PrimaryActorTick.bCanEverTick = false;

	InventoryComponent = CreateDefaultSubobject<UDkInventoryComponent>(TEXT("DkInventoryComponent"));
}

