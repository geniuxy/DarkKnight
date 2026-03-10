// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkCharacterBase.h"
#include "DkRewardBox.generated.h"

class UBoxComponent;

UCLASS()
class DARKKNIGHT_API ADkRewardBox : public ADkCharacterBase
{
	GENERATED_BODY()

public:
	ADkRewardBox();

protected:
	virtual void BeginPlay() override;

	/************************************************************/
	/*						交互后获得Item						*/
	/************************************************************/
protected:
	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UBoxComponent> BoxComponent;

	UFUNCTION()
	void HandleBoxOverlapped(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);
};
