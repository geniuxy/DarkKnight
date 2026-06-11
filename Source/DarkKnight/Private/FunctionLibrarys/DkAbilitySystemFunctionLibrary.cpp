// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrarys/DkAbilitySystemFunctionLibrary.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "DkGameplayTags.h"

bool UDkAbilitySystemFunctionLibrary::IsActorDead(const AActor* ActorToCheck)
{
	return ActorHasTag(ActorToCheck, DkGameplayTags::Dk_Stats_Dead);
}

bool UDkAbilitySystemFunctionLibrary::ActorHasTag(const AActor* ActorToCheck, const FGameplayTag& Tag)
{
	if (const IAbilitySystemInterface* ActorASI = Cast<IAbilitySystemInterface>(ActorToCheck))
	{
		if (UAbilitySystemComponent* ActorASC = ActorASI->GetAbilitySystemComponent())
		{
			return ActorASC->HasMatchingGameplayTag(Tag);
		}
	}
	return false;
}
