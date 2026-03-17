// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkActionComponent.h"

#include "Characters/DkCharacterHero.h"
#include "DataAssets/CharacterInfo.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerStates/DkPlayerStateBase.h"

UDkActionComponent::UDkActionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDkActionComponent::InitializeActorComponent(UCharacterInfo* InCharacterInfo)
{
	OwnerCharacter = Cast<ADkCharacterBase>(GetOwner());
	if (OwnerCharacter->ActorHasTag(TEXT("Player")))
	{
		SetOwnerType(EOwnerType::Player);
	}
	else if (OwnerCharacter->ActorHasTag(TEXT("Monster")))
	{
		if (InCharacterInfo->MonsterType == EMonsterType::Dragon)
		{
			SetOwnerType(EOwnerType::Dragon);
		}
	}
	else
	{
		SetOwnerType(EOwnerType::NPC);
	}
}

void UDkActionComponent::SetCurrentActionState(EActionState InActionState)
{
	LastActionState = CurrentActionState;
	CurrentActionState = InActionState;

	if (OwnerType == EOwnerType::Player)
	{
		ADkCharacterHero* OwningPlayer = Cast<ADkCharacterHero>(GetOwner());
		ADkPlayerStateBase* OwnerPlayerState = Cast<ADkPlayerStateBase>(OwningPlayer->GetPlayerState());
		OwnerPlayerState->SetCurrentActionState(CurrentActionState);
	}

	switch (CurrentActionState)
	{
	case EActionState::OutOfCombat:
		bIsInCombat = false;
		break;
	case EActionState::InCombat:
		bIsInCombat = true;
		break;
	case EActionState::CombatFinisherAttacker:
		break;
	case EActionState::CombatFinisherReceiver:
		break;
	case EActionState::Death:
		break;
	default:
		break;
	}
}

void UDkActionComponent::BeginPlay()
{
	Super::BeginPlay();

	ADkCharacterHero* Hero = CastChecked<ADkCharacterHero>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (!IsValid(Hero)) return;

	
}
