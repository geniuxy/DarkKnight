// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DkCharacterHero.h"

#include "Camera/CameraComponent.h"
#include "Components/DkEquipmentComponent.h"
#include "Components/DkInventoryComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/DkAbilitySystemComponent.h"
#include "PlayerStates/DkPlayerStateBase.h"


class ADkPlayerStateBase;

ADkCharacterHero::ADkCharacterHero()
{
	PrimaryActorTick.bCanEverTick = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = FVector(0.f, 80.f, 50.f);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	InventoryComponent = CreateDefaultSubobject<UDkInventoryComponent>(TEXT("DkInventoryComponent"));
	EquipmentComponent = CreateDefaultSubobject<UDkEquipmentComponent>(TEXT("DkEquipmentComponent"));
}

void ADkCharacterHero::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();

	SwitchLocomotionStyle(ELocomotionStyle::Walk);
}

void ADkCharacterHero::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
	
	SwitchLocomotionStyle(ELocomotionStyle::Walk);
}

void ADkCharacterHero::InitAbilityActorInfo()
{
	ADkPlayerStateBase* OwningPlayerState = GetPlayerState<ADkPlayerStateBase>();
	check(OwningPlayerState);
	OwningPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(OwningPlayerState, this);
	OwningPlayerState->GetAbilitySystemComponent()->AbilityActorInfoSet();
	AbilitySystemComponent = OwningPlayerState->GetAbilitySystemComponent();
	AttributeSet = OwningPlayerState->GetAttributeSet();
}

