// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AnimNotifies/AN_CommitTask.h"

#include "Characters/DkCharacterHero.h"
#include "Games/PlayerStates/DkPlayerStateBase.h"

void UAN_CommitTask::Notify(
	USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp->GetOwner()) return;

	ADkCharacterHero* OwnerCharacter = Cast<ADkCharacterHero>(MeshComp->GetOwner());
	if (!OwnerCharacter || !OwnerCharacter->HasAuthority()) return;

	if (ADkPlayerStateBase* OwnerPlayerState = OwnerCharacter->GetPlayerState<ADkPlayerStateBase>())
	{
		OwnerPlayerState->OnCommitTaskDelegate.Broadcast(MainTaskId, SubTaskId, CommitCount);
	}
}
