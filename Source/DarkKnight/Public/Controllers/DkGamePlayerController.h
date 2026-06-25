// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "Interfaces/LoadingScreenInterface.h"
#include "DkGamePlayerController.generated.h"

class ADkCharacterBase;
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
class DARKKNIGHT_API ADkGamePlayerController : public APlayerController, public ILoadingScreenInterface,
                                               public IGenericTeamAgentInterface
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

	// 只在Server上执行
	virtual void OnPossess(APawn* NewPawn) override;
	// 只在Client上执行,也会在Listen Server(P2P 没有玩家当主机)上执行(其中一个玩家同时充当"服务器"角色)
	virtual void AcknowledgePossession(APawn* NewPawn) override;

	// virtual void OnRep_Pawn() override;

	virtual void SetupInputComponent() override;

protected:
	UPROPERTY()
	TObjectPtr<ADkCharacterBase> OwningPlayerCharacter;

	FGenericTeamId TeamID;

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
private:
	void TraceForItem();
	FHitResult CursorHit;
	TWeakObjectPtr<AActor> ThisActor;
	TWeakObjectPtr<AActor> LastActor;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	double TraceLength;

	UPROPERTY()
	TObjectPtr<UDkWidgetInteractScreen> CachedInteractScreen;

public:
	FORCEINLINE UDkWidgetInteractScreen* GetInteractScreen() const { return CachedInteractScreen; }
	/********/

	/* 仓库 */
private:
	void RefreshInventoryComponent();
	TWeakObjectPtr<UDkInventoryComponent> InventoryComponent;
	/********/

	UPROPERTY(EditDefaultsOnly, Category = "Primary")
	TSubclassOf<UDkWidgetPrimaryLayout> PrimaryLayoutClass;

	UPROPERTY()
	TObjectPtr<UDkWidgetPrimaryLayout> PrimaryLayoutWidget;
};
