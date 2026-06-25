// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DkDialogComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKKNIGHT_API UDkDialogComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDkDialogComponent();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Interact")
	FString InteractMessage;

public:
	FORCEINLINE FString GetInteractMessage() const { return InteractMessage; }
};
