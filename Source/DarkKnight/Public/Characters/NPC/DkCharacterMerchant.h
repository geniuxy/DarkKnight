// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkCharacterNPC.h"
#include "DkCharacterMerchant.generated.h"

UCLASS()
class DARKKNIGHT_API ADkCharacterMerchant : public ADkCharacterNPC
{
	GENERATED_BODY()

public:
	ADkCharacterMerchant();

protected:
	virtual void BeginPlay() override;
};
