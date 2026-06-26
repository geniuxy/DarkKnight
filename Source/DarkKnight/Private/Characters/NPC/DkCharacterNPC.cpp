// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/DkCharacterNPC.h"

#include "Characters/DkCharacterHero.h"
#include "Components/BoxComponent.h"
#include "Components/DkDialogComponent.h"
#include "Controllers/DkGamePlayerController.h"
#include "Widgets/Interact/DkWidgetInteractScreen.h"


ADkCharacterNPC::ADkCharacterNPC()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleBoxOverlapped);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::HandleBoxEndOverlap);

	DialogComponent = CreateDefaultSubobject<UDkDialogComponent>(TEXT("DkDialogComponent"));
}

void ADkCharacterNPC::BeginPlay()
{
	Super::BeginPlay();
}

void ADkCharacterNPC::HandleBoxOverlapped(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!IsValid(DialogComponent)) return;
	if (OtherActor == this) return;
	ADkCharacterHero* OverlappedCharacter = Cast<ADkCharacterHero>(OtherActor);
	if (!OverlappedCharacter) return;

	ADkGamePlayerController* PC = OverlappedCharacter->GetController<ADkGamePlayerController>();
	if (!IsValid(PC)) return;

	if (IsValid(PC->GetInteractScreen()))
	{
		PC->GetInteractScreen()->ShowInteractMessage(DialogComponent->GetInteractMessage());
		PC->SetInteractiveNPC(this);
	}
}

void ADkCharacterNPC::HandleBoxEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int OtherBodyIndex)
{
	if (!IsValid(DialogComponent)) return;
	if (OtherActor == this) return;
	ADkCharacterHero* OverlappedCharacter = Cast<ADkCharacterHero>(OtherActor);
	if (!OverlappedCharacter) return;

	ADkGamePlayerController* PC = OverlappedCharacter->GetController<ADkGamePlayerController>();
	if (!IsValid(PC)) return;

	if (IsValid(PC->GetInteractScreen()))
	{
		PC->GetInteractScreen()->HideInteractMessage();
		PC->SetInteractiveNPC(nullptr);
	}
}
