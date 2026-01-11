// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DkItemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DARKKNIGHT_API UDkItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDkItemComponent();

private:
	UPROPERTY(EditAnywhere, Category="Inventory")
	FString PickUpMessage;

public:
	FORCEINLINE FString GetPickUpMessage() const { return PickUpMessage; }
};
