// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DkNPCDialogComponent.generated.h"


class UGameplayEffect;
class UDkWidgetDialogScreen;
class ADkGamePlayerController;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKKNIGHT_API UDkNPCDialogComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDkNPCDialogComponent();

	bool CanStartDialog();
	void TryStartDialog(ADkGamePlayerController* PC);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Interact")
	FString InteractMessage;

	UPROPERTY(EditAnywhere, Category="Dialog Details")
	int DialogId = 0;

	UPROPERTY(EditAnywhere, Category="Dialog Details")
	int NPCId = 0;
	
	UPROPERTY(EditAnywhere, Category="Dialog Details")
	UGameplayEffect* DialogStatsEffect;

	UPROPERTY()
	TObjectPtr<UDkWidgetDialogScreen> CachedDialogScreen;

public:
	FORCEINLINE FString GetInteractMessage() const { return InteractMessage; }
};
