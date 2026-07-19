// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/DkStateTreeAIController.h"

#include "DarkKnightDebugHelper.h"
#include "DkGameplayTags.h"
#include "Characters/DkCharacterBase.h"
#include "Components/DkStateTreeAIComponent.h"
#include "Components/StateTreeAIComponent.h"


class ADkPickUpActorBase;

ADkStateTreeAIController::ADkStateTreeAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	StateTreeComponent = CreateDefaultSubobject<UDkStateTreeAIComponent>(TEXT("StateTreeAIComp"));
	StateTreeComponent->SetStartLogicAutomatically(false);
}

void ADkStateTreeAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!IsValid(InPawn))
	{
		return;
	}

	OwningCharacter = Cast<ADkCharacterBase>(InPawn);
	if (!OwningCharacter) return;

	if (StateTreeComponent)
	{
		StateTreeComponent->StartLogic();
	}
	Debug::Print(TEXT("StateTreeComponent IsRunning"), StateTreeComponent->IsRunning() ? TEXT("Yes") : TEXT("No"));
	
	FTimerHandle DelaySleepHandle;
	GetWorld()->GetTimerManager().SetTimer(DelaySleepHandle, FTimerDelegate::CreateLambda([this]()
	{
		StateTreeComponent->SendStateTreeEvent(DkGameplayTags::Dk_StateTree_Event_Sleep);
	}), 0.2f, false);

	FTimerHandle DelayWorkHandle;
	GetWorld()->GetTimerManager().SetTimer(DelayWorkHandle, FTimerDelegate::CreateLambda([this]()
	{
		StateTreeComponent->SendStateTreeEvent(DkGameplayTags::Dk_StateTree_Event_Work);
	}), 5.f, false);
}

void ADkStateTreeAIController::OnUnPossess()
{
	OwningCharacter = nullptr;
	if (StateTreeComponent)
	{
		StateTreeComponent->StopLogic("UnPossess");
	}
	Super::OnUnPossess();
}
