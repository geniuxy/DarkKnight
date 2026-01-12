// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkCharacterBase.h"
#include "DkCharacterHero.generated.h"

class UDkInventoryComponent;

UCLASS()
class DARKKNIGHT_API ADkCharacterHero : public ADkCharacterBase
{
	GENERATED_BODY()

public:
	ADkCharacterHero();

protected:
	/* Actor Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Inventory")
	UDkInventoryComponent* DkInventoryComponent;
	/*********/
};
