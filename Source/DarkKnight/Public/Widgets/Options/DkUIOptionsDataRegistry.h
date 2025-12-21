// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DkUIOptionsDataRegistry.generated.h"

class UDkUIListDataObjectCollection;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkUIOptionsDataRegistry : public UObject
{
	GENERATED_BODY()

public:
	// 在创建 UOptionsDataRegistry 类型的对象之后，OptionScreen会立即调用此方法。
	void InitOptionsDataRegister(ULocalPlayer* InOwningLocalPlayer);

private:
	void InitGameplayCollectionTab();
	void InitAudioCollectionTab();
	void InitVideoCollectionTab();
	void InitControlCollectionTab();

	UPROPERTY(Transient)
	TArray<UDkUIListDataObjectCollection*> RegisteredOptionsTabCollections;

public:
	FORCEINLINE const TArray<UDkUIListDataObjectCollection*>& GetRegisteredOptionsTabCollections() const
	{
		return RegisteredOptionsTabCollections;
	}
};
