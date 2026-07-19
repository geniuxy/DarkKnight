// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DkStateTreeAIController.generated.h"

class ADkCharacterBase;
class UDkStateTreeAIComponent;
class UStateTreeAIComponent;

UCLASS()
class DARKKNIGHT_API ADkStateTreeAIController : public AAIController
{
	GENERATED_BODY()

public:
	ADkStateTreeAIController();

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	UPROPERTY()
	ADkCharacterBase* OwningCharacter;

	/**********************************************************************/
	/*                             StateTree                              */
	/**********************************************************************/
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDkStateTreeAIComponent* StateTreeComponent;

public:
	FORCEINLINE UDkStateTreeAIComponent* GetStateTreeComponent() const { return StateTreeComponent; }
};
