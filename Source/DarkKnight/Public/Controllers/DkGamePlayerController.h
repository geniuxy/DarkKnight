// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/LoadingScreenInterface.h"
#include "DkGamePlayerController.generated.h"

class UDkInventoryComponent;
class UDkWidgetInteractScreen;
class UDkWidgetPrimaryLayout;
class UInputAction;
class UInputMappingContext;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API ADkGamePlayerController : public APlayerController, public ILoadingScreenInterface
{
	GENERATED_BODY()

public:
	ADkGamePlayerController();
	virtual void Tick(float DeltaSeconds) override;

	//~Begin ILoadingScreenInterface Interface
	virtual void OnLoadingScreenDeactivated_Implementation() override;
	//~End ILoadingScreenInterface Interface

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:
	/* 交互 */
	void OnInteract();

	UPROPERTY(EditDefaultsOnly, Category="Interact")
	TObjectPtr<UInputAction> InteractAction;
	/********/

	/* 捡拾物品 */
	void TraceForItem();
	FHitResult CursorHit;
	TWeakObjectPtr<AActor> ThisActor;
	TWeakObjectPtr<AActor> LastActor;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	double TraceLength;

	UPROPERTY()
	TObjectPtr<UDkWidgetInteractScreen> CachedInteractScreen;
	/********/

	/* 仓库 */
	void OnInventoryActionTriggered();
	
	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TObjectPtr<UInputAction> InventoryAction;

	TWeakObjectPtr<UDkInventoryComponent> InventoryComponent;
	/********/

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TObjectPtr<UInputMappingContext> IMCGamePlay;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UDkWidgetPrimaryLayout> PrimaryLayoutClass;

	UPROPERTY()
	TObjectPtr<UDkWidgetPrimaryLayout> PrimaryLayoutWidget;
};
