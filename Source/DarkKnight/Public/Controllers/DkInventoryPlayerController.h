// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/LoadingScreenInterface.h"
#include "DkInventoryPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API ADkInventoryPlayerController : public APlayerController, public ILoadingScreenInterface
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:
	void OnInteract();

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TObjectPtr<UInputMappingContext> IMCInventory;
	
	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TObjectPtr<UInputAction> InteractAction;
};
