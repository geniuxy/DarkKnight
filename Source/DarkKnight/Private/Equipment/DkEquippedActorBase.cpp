// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/DkEquippedActorBase.h"


ADkEquippedActorBase::ADkEquippedActorBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}
