// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkUIListDataObjectBase.h"
#include "DkUIListDataObjectCollection.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkUIListDataObjectCollection : public UDkUIListDataObjectBase
{
	GENERATED_BODY()

public:
	void AddChildListData(UDkUIListDataObjectBase* InChildListData);

	//~Begin UDkUIListDataObjectBase Function
	virtual TArray<UDkUIListDataObjectBase*> GetAllChildSettingData() const;
	virtual bool HasAnyChildListData() const;
	//~End UDkUIListDataObjectBase Function

private:
	UPROPERTY(Transient)
	TArray<UDkUIListDataObjectBase*> ChildListDataArray;
};
