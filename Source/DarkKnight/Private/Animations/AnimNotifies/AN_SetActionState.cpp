// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotifies/AN_SetActionState.h"

#include "Components/DkActionComponent.h"

void UAN_SetActionState::Notify(
	USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UDkActionComponent* OwnerActionComp =
		Cast<UDkActionComponent>(MeshComp->GetOwner()->GetComponentByClass(UDkActionComponent::StaticClass()));
	if (OwnerActionComp)
	{
		OwnerActionComp->SetCurrentActionState(ActionState);
	}
}
