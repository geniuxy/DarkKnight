// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotifies/AN_ApplyGameplayEffect.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameplayEffect.h"
#include "GAS/DkAbilitySystemComponent.h"

void UAN_ApplyGameplayEffect::Notify(
	USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp->GetOwner()) return;

	UAbilitySystemComponent* OwnerASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(MeshComp->GetOwner());
	if (!OwnerASC) return;

	if (Effect && MeshComp->GetOwner()->HasAuthority())
	{
		if (UDkAbilitySystemComponent* OwnerDkASC = Cast<UDkAbilitySystemComponent>(OwnerASC))
		{
			OwnerDkASC->AuthApplyGameplayEffect(Effect);
		}
	}
}
