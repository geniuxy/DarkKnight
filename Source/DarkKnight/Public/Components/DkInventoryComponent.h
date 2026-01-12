// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DkInventoryComponent.generated.h"


class ADkCharacterHero;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class DARKKNIGHT_API UDkInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UDkInventoryComponent();

protected:
	virtual void BeginPlay() override;


private:
	TWeakObjectPtr<ADkCharacterHero> OwningCharacter;
	
	void ConstructInventoryMenu();
};
