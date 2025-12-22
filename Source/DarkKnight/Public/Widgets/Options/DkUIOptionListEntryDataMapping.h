// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "DkUIOptionListEntryDataMapping.generated.h"

class UDkUIListDataObjectBase;
class UDkUIWidgetListEntryBase;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkUIOptionListEntryDataMapping : public UDataAsset
{
	GENERATED_BODY()

public:
	TSubclassOf<UDkUIWidgetListEntryBase> FindEntryWidgetClassByDataObject(UDkUIListDataObjectBase* InDataObject) const;

private:
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UDkUIListDataObjectBase>, TSubclassOf<UDkUIWidgetListEntryBase>> DataObjectListEntryMap;
};
