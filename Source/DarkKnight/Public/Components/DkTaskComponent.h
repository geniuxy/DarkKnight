// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DkTypes/DkStructs.h"
#include "DkTaskComponent.generated.h"

class ADkPlayerStateBase;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKKNIGHT_API UDkTaskComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDkTaskComponent();

	void UpdatePlayerTaskCompletionStatus();

	void AcceptTask(int InTaskId);
	void UpdateTask(int InTaskId, int InSubTaskId);
	void CompleteTask(int InTaskId);

	bool IsTaskFinished(int InTaskId) const;

	ETaskState GetSubTaskState(int InMainTaskId, int InSubTaskId) const;

	int GetSubTaskProgress(int InMainTaskId, int InSubTaskId) const;

	TArray<int> GetAllPlayerTaskId() const;
	
	FText GetSubTaskDescription(int InMainTaskId, int InSubTaskId) const;

	int GetCurSubTaskId(int InMainTaskId) const;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<ADkPlayerStateBase> OwnerPlayerState;
	
	TMap<int, FTaskCompletionStatus> CurrentTaskCompletionStatus;

	FGameplayTagContainer CachedTaskTags;

	bool HasFinishedAllPreconditionTask(const FGameplayTagContainer& InTagContainer) const;
	void UpdateTaskState(int InTaskId, ETaskState InTaskState);
	void UpdateSubTaskState(int InMainTaskId, int InSubTaskId, ETaskState InTaskState);
	bool IsNextSubTaskIdZero(int InMainTaskId, int InSubTaskId) const;
};
