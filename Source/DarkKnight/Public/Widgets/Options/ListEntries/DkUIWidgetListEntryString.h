// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/ListEntries/DkUIWidgetListEntryBase.h"
#include "DkUIWidgetListEntryString.generated.h"

class UDkUICommonRotator;
class UDkUICommonButtonBase;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkUIWidgetListEntryString : public UDkUIWidgetListEntryBase
{
	GENERATED_BODY()

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UDkUICommonButtonBase* CommonButton_Previous;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UDkUICommonRotator* CommonRotator_AvailableOptions;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	UDkUICommonButtonBase* CommonButton_Next;
	//***** Bound Widgets *****//
};
