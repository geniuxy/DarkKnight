// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DkUIListDataObjectBase.generated.h"

#define LIST_DATA_ACCESSOR(DataType, PropertyName) \
	FORCEINLINE DataType Get##PropertyName() const { return PropertyName; } \
	void Set##PropertyName(DataType In##PropertyName) { PropertyName = In##PropertyName; }

/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkUIListDataObjectBase : public UObject
{
	GENERATED_BODY()

public:
	LIST_DATA_ACCESSOR(FName, DataID);
	LIST_DATA_ACCESSOR(FText, DataDisplayName);
	LIST_DATA_ACCESSOR(FText, DescriptionRichText);
	LIST_DATA_ACCESSOR(FText, DisabledRichText);
	LIST_DATA_ACCESSOR(TSoftObjectPtr<UTexture2D>, SoftDescriptionImage);
	LIST_DATA_ACCESSOR(UDkUIListDataObjectBase*, ParentData);

	// 基类中为空。子类ListDataObject_Collection应重写它。
	// 该函数应返回选项卡中的所有子数据
	virtual TArray<UDkUIListDataObjectBase*> GetChildSettingData() const { return TArray<UDkUIListDataObjectBase*>(); }
	
private:
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledRichText;
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;

	UPROPERTY(Transient)
	UDkUIListDataObjectBase* ParentData;
};
