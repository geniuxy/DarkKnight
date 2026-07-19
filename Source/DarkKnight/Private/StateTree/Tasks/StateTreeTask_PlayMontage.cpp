#include "StateTree/Tasks/StateTreeTask_PlayMontage.h"

#include "AIController.h"
#include "DarkKnightDebugHelper.h"
#include "StateTreeExecutionContext.h"
#include "Characters/DkCharacterBase.h"

EStateTreeRunStatus FStateTreeTask_PlayMontage::EnterState(
	FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
	
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);
	if (!InstanceData.OwningCharacter)
	{
		Debug::Print(TEXT("FStateTreeTask_PlayMontage failed since OwningCharacter is missing."));
		return EStateTreeRunStatus::Failed;
	}
	if (!InstanceData.MontageToPlay)
	{
		Debug::Print(TEXT("FStateTreeTask_PlayMontage failed since MontageToPlay is missing."));
		return EStateTreeRunStatus::Failed;
	}
	UAnimInstance* OwningAnimInstance = InstanceData.OwningCharacter->GetMesh()->GetAnimInstance();
	if (!OwningAnimInstance)
	{
		Debug::Print(TEXT("FStateTreeTask_PlayMontage failed since AnimInstance is missing."));
		return EStateTreeRunStatus::Failed;
	}

	RunStatus = EStateTreeRunStatus::Running;

	FOnMontageEnded EndDelegate;
	EndDelegate.BindRaw(this, &FStateTreeTask_PlayMontage::HandleMontageEnded);

	InstanceData.OwningCharacter->PlayAnimMontage(InstanceData.MontageToPlay);

	OwningAnimInstance->Montage_SetEndDelegate(EndDelegate, InstanceData.MontageToPlay);
	
	return RunStatus;
}

EStateTreeRunStatus FStateTreeTask_PlayMontage::Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const
{
	return RunStatus;
}

void FStateTreeTask_PlayMontage::HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted) const
{
	if (!bInterrupted)
	{
		RunStatus = EStateTreeRunStatus::Succeeded;
	}
	else
	{
		RunStatus = EStateTreeRunStatus::Failed;
	}
}
