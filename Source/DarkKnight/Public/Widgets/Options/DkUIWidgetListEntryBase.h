// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "DkUIWidgetListEntryBase.generated.h"

class UCommonTextBlock;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, meta = (DisableNativeTick))
class DARKKNIGHT_API UDkUIWidgetListEntryBase : public UCommonUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()

protected:
	//~Begin IUserObjectListEntry Function
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	//~End IUserObjectListEntry Function

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetOptional, AllowPrivateAccess="true"))
	UCommonTextBlock* CommonText_SettingDisplayName;
	//***** Bound Widgets *****//
};
