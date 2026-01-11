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

public:
	ADkInventoryPlayerController();
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:
	void OnInteract();

	/* 捡拾物品 */
	void TraceForItem();
	FHitResult CursorHit;
	TWeakObjectPtr<AActor> ThisActor;
	TWeakObjectPtr<AActor> LastActor;
	
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	double TraceLength;
	/********/

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TObjectPtr<UInputMappingContext> IMCInventory;
	
	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TObjectPtr<UInputAction> InteractAction;
};
