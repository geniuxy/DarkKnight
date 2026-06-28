// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/DkCharacterBase.h"
#include "DkCharacterNPC.generated.h"

class UDkNpcDialogComponent;
class UBoxComponent;

UCLASS()
class DARKKNIGHT_API ADkCharacterNPC : public ADkCharacterBase
{
	GENERATED_BODY()

public:
	ADkCharacterNPC();

protected:
	virtual void BeginPlay() override;

	/************************************************************/
	/*                  进入范围后显示可对话按键标识                 */
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

	UFUNCTION()
	void HandleBoxEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int OtherBodyIndex
	);
	/**********************************************************************/
	/*                              Dialog                                */
	/**********************************************************************/
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UDkNpcDialogComponent* NpcDialogComponent;

public:
	FORCEINLINE UDkNpcDialogComponent* GetNpcDialogComponent() const { return NpcDialogComponent; }
};
