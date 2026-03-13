// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/LoadingScreenInterface.h"
#include "DkGamePlayerController.generated.h"

class UInputConfig;
struct FInputActionValue;
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

	virtual void OnPossess(APawn* InPawn) override;

	virtual void OnRep_Pawn() override;

	virtual void SetupInputComponent() override;

private:
	/* Input Actions */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input Config", meta=(AllowPrivateAccess="true"))
	UInputConfig* InputConfigDataAsset;
	/********/

	/* Input Action CallBack */
	void HandleGroundMovementInput(const FInputActionValue& InputActionValue);
	void ToggleMovementStyle();
	void OnLookTriggered(const FInputActionValue& InputActionValue);
	void OnJumpPressed();
	void OnInteract();
	void OnOpenSystemMenu();
	void OnInventoryActionTriggered();
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
	void RefreshInventoryComponent();
	TWeakObjectPtr<UDkInventoryComponent> InventoryComponent;
	/********/

	UPROPERTY(EditDefaultsOnly, Category= "Primary")
	TObjectPtr<UInputMappingContext> IMCGamePlay;

	UPROPERTY(EditDefaultsOnly, Category = "Primary")
	TSubclassOf<UDkWidgetPrimaryLayout> PrimaryLayoutClass;

	UPROPERTY()
	TObjectPtr<UDkWidgetPrimaryLayout> PrimaryLayoutWidget;
};
