// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "DkGameMenuCenterAreaCraft.generated.h"

class UCommonListView;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkGameMenuCenterAreaCraft : public UDkWidgetActivatableBase
{
	GENERATED_BODY()

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonListView* CraftItemListView;
	//***** Bound Widgets *****//
};
