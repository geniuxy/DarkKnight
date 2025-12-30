// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Options/DataObjects/DkUIListDataObjectString.h"
#include "DkUIListDataObjectStrResolution.generated.h"

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkUIListDataObjectStrResolution : public UDkUIListDataObjectString
{
	GENERATED_BODY()

public:
	void InitResolutionValues();

protected:
	//~Begin UDkUIListDataObjectBase Function
	virtual void OnDataObjectInitialized() override;
	//~End UDkUIListDataObjectBase Function

private:
	FString ResToStringValue(const FIntPoint& InResolution) const;
	FText ResToDisplayText(const FIntPoint& InResolution) const;

	FString MaxAllowedResolution;

public:
	FORCEINLINE FString GetMaxAllowedResolution() const { return MaxAllowedResolution; }
};
