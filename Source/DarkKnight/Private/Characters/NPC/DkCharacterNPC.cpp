// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NPC/DkCharacterNPC.h"

#include "Characters/DkCharacterHero.h"
#include "Components/BoxComponent.h"
#include "Components/DkNpcDialogComponent.h"
#include "Controllers/DkGamePlayerController.h"
#include "FunctionLibrarys/DkAbilitySystemFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widgets/Interact/DkWidgetInteractScreen.h"


ADkCharacterNPC::ADkCharacterNPC()
{
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(GetRootComponent());
	BoxComponent->SetBoxExtent(FVector(100.f));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::HandleBoxOverlapped);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &ThisClass::HandleBoxEndOverlap);

	GetCharacterMovement()->GetNavMovementProperties()->bUseAccelerationForPaths = true;

	NpcDialogComponent = CreateDefaultSubobject<UDkNpcDialogComponent>(TEXT("NpcDialogComponent"));
}

void ADkCharacterNPC::BeginPlay()
{
	Super::BeginPlay();

	SetGenericTeamId(3);
}

void ADkCharacterNPC::HandleBoxOverlapped(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!IsValid(NpcDialogComponent)) return;
	if (!NpcDialogComponent->CanStartDialog()) return;
	if (OtherActor == this) return;
	ADkCharacterHero* OverlappedCharacter = Cast<ADkCharacterHero>(OtherActor);
	if (!OverlappedCharacter) return;
	if (UDkAbilitySystemFunctionLibrary::IsActorDead(OverlappedCharacter)) return;

	ADkGamePlayerController* PC = OverlappedCharacter->GetController<ADkGamePlayerController>();
	if (!IsValid(PC)) return;

	if (IsValid(PC->GetInteractScreen()))
	{
		PC->GetInteractScreen()->ShowInteractMessage(NpcDialogComponent->GetInteractMessage());
		PC->SetInteractiveNPC(this);
	}
}

void ADkCharacterNPC::HandleBoxEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int OtherBodyIndex)
{
	if (!IsValid(NpcDialogComponent)) return;
	if (!NpcDialogComponent->CanStartDialog()) return;
	if (OtherActor == this) return;
	ADkCharacterHero* OverlappedCharacter = Cast<ADkCharacterHero>(OtherActor);
	if (!OverlappedCharacter) return;
	if (UDkAbilitySystemFunctionLibrary::IsActorDead(OverlappedCharacter)) return;

	ADkGamePlayerController* PC = OverlappedCharacter->GetController<ADkGamePlayerController>();
	if (!IsValid(PC)) return;

	if (IsValid(PC->GetInteractScreen()))
	{
		PC->GetInteractScreen()->HideInteractMessage();
		PC->SetInteractiveNPC(nullptr);
	}
}
