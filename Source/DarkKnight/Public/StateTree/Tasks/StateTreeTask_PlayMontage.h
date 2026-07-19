#pragma once

#include "Tasks/StateTreeAITask.h"

#include "StateTreeTask_PlayMontage.generated.h"

class ADkCharacterBase;
class AAIController;

USTRUCT()
struct FStateTreeTask_InstanceData_PlayMontage
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Context)
	TObjectPtr<ADkCharacterBase> OwningCharacter;

	UPROPERTY(EditAnywhere, Category = Parameter)
	TObjectPtr<UAnimMontage> MontageToPlay = nullptr;
};

USTRUCT(meta = (DisplayName = "Dk Play Montage", Category = "AI|Action"))
struct FStateTreeTask_PlayMontage : public FStateTreeAIActionTaskBase
{
	GENERATED_BODY()

	using FInstanceDataType = FStateTreeTask_InstanceData_PlayMontage;

	virtual const UStruct* GetInstanceDataType() const override { return FInstanceDataType::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(
		FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition
	) const override;

	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) const override;

	void HandleMontageEnded(UAnimMontage* Montage, bool bInterrupted) const;

	mutable EStateTreeRunStatus RunStatus = EStateTreeRunStatus::Running;
};
