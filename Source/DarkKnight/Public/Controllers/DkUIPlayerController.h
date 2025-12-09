// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DkUIPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API ADkUIPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	//~Begin APlayerController Fucntion
	virtual void OnPossess(APawn* InPawn) override;
	//~End APlayerController Fucntion
};
