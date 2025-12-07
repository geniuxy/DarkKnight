// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DkUISubsystem.generated.h"

class UDkWidgetActivatableBase;
struct FGameplayTag;
class UDkWidgetPrimaryLayout;

enum class EAsyncPushWidgetState: uint8
{
	OnCreatedBeforePush,
	AfterPush
};

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	static UDkUISubsystem* Get(const UObject* WorldContextObject);

	//~Begin USubsystem Function
	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	//~End USubsystem Function

	UFUNCTION(BlueprintCallable)
	void RegisterCreatedPrimaryLayoutWidget(UDkWidgetPrimaryLayout* InCreatedWidget);

	void PushSoftWidgetToStackAsync(
		const FGameplayTag& InWidgetStackTag,
		TSoftClassPtr<UDkWidgetActivatableBase> InSoftWidgetClass,
		TFunction<void(EAsyncPushWidgetState, UDkWidgetActivatableBase*)> AsyncPushStateCallback
	);

private:
	UPROPERTY(Transient)
	UDkWidgetPrimaryLayout* CreatedPrimaryLayout;
};
