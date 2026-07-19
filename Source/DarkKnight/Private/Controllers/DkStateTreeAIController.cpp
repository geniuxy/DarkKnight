// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/DkStateTreeAIController.h"

#include "DarkKnightDebugHelper.h"
#include "DkGameplayTags.h"
#include "Components/DkStateTreeAIComponent.h"
#include "Components/StateTreeAIComponent.h"


ADkStateTreeAIController::ADkStateTreeAIController()
{
	PrimaryActorTick.bCanEverTick = true;

	StateTreeComponent = CreateDefaultSubobject<UDkStateTreeAIComponent>(TEXT("StateTreeAIComp"));
}


void ADkStateTreeAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (!IsValid(InPawn))
	{
		return;
	}

	if (StateTreeComponent)
	{
		StateTreeComponent->StartLogic();
	}

	FTimerHandle DelayWorkHandle;
	GetWorld()->GetTimerManager().SetTimer(DelayWorkHandle, FTimerDelegate::CreateLambda([this]()
	{
		StateTreeComponent->SendStateTreeEvent(DkGameplayTags::Dk_StateTree_Event_Work);
	}), 4.f, false);
}

void ADkStateTreeAIController::OnUnPossess()
{
	if (StateTreeComponent)
	{
		StateTreeComponent->StopLogic("UnPossess");
	}
	Super::OnUnPossess();
}