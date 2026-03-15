// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DkRewardBox.h"

#include "Components/BoxComponent.h"


ADkRewardBox::ADkRewardBox()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleBoxOverlapped);
}

void ADkRewardBox::BeginPlay()
{
	Super::BeginPlay();
}

void ADkRewardBox::HandleBoxOverlapped(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (OtherActor == this) return;
	ServerSpawnRewardItemActor();
}
