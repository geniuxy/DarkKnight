// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkPlayerDialogComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Subsytems/EngineSubsystems/DkDataSubsystem.h"

UDkPlayerDialogComponent::UDkPlayerDialogComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CachedDialogTags.Reset();
}

bool UDkPlayerDialogComponent::FindDialogGameplayTag(FGameplayTagContainer InTags)
{
	return CachedDialogTags.HasAllExact(InTags);
}

void UDkPlayerDialogComponent::AddDialogTag(FGameplayTag InTag)
{
	CachedDialogTags.AddTag(InTag);
}

void UDkPlayerDialogComponent::BeginPlay()
{
	Super::BeginPlay();

	// 更新对话相关信息，Id=1为玩家自己
	if (GetOwner() == UGameplayStatics::GetPlayerPawn(this, 0))
	{
		UDkDataSubsystem::Get()->UpdateNpcInfo(1, GetOwner());
	}
}
