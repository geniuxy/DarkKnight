// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DkUISubsystem.generated.h"

class UDkUICommonButtonBase;
class UDkWidgetActivatableBase;
struct FGameplayTag;
class UDkWidgetPrimaryLayout;

enum class EAsyncPushWidgetState: uint8
{
	OnCreatedBeforePush,
	AfterPush
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnButtonDescriptionTextUpdatedDelegate, UDkUICommonButtonBase*, Button, FText, DescriptionText
);

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

	UPROPERTY(BlueprintAssignable)
	FOnButtonDescriptionTextUpdatedDelegate OnButtonDescriptionTextUpdated;

private:
	UPROPERTY(Transient)
	UDkWidgetPrimaryLayout* CreatedPrimaryLayout;
};
