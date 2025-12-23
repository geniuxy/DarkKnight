// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkTypes/DkEnums.h"
#include "UObject/Object.h"
#include "DkUIListDataObjectBase.generated.h"


#define LIST_DATA_ACCESSOR(DataType, PropertyName) \
	FORCEINLINE DataType Get##PropertyName() const { return PropertyName; } \
	void Set##PropertyName(DataType In##PropertyName) { PropertyName = In##PropertyName; }

/**
 * 
 */
UCLASS(Abstract)
class DARKKNIGHT_API UDkUIListDataObjectBase : public UObject
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnListDataModifiedDelegate,
	                                     UDkUIListDataObjectBase*, EOptionsListDataModifyReason)
	FOnListDataModifiedDelegate OnListDataModified;

	LIST_DATA_ACCESSOR(FName, DataID);
	LIST_DATA_ACCESSOR(FText, DataDisplayName);
	LIST_DATA_ACCESSOR(FText, DescriptionRichText);
	LIST_DATA_ACCESSOR(FText, DisabledRichText);
	LIST_DATA_ACCESSOR(TSoftObjectPtr<UTexture2D>, SoftDescriptionImage);
	LIST_DATA_ACCESSOR(UDkUIListDataObjectBase*, ParentData);

	void InitDataObject();

	// 基类中为空。子类ListDataObject_Collection应重写它。
	// 该函数应返回选项卡中的所有子数据
	virtual TArray<UDkUIListDataObjectBase*> GetAllChildSettingData() const
	{
		return TArray<UDkUIListDataObjectBase*>();
	}

	virtual bool HasAnyChildListData() const { return false; }

protected:
	// 基类中为空。子类应重写它，以相应地处理所需的初始化
	virtual void OnDataObjectInitialized();

	virtual void NotifyListDataModified(
		UDkUIListDataObjectBase* ModifiedData,
		EOptionsListDataModifyReason ModifyReason = EOptionsListDataModifyReason::DirectlyModified
	);

private:
	FName DataID;
	FText DataDisplayName;
	FText DescriptionRichText;
	FText DisabledRichText;
	TSoftObjectPtr<UTexture2D> SoftDescriptionImage;

	UPROPERTY(Transient)
	UDkUIListDataObjectBase* ParentData;

	bool bShouldApplyChangeImmediately = false;

public:
	void SetShouldApplyChangeImmediately(bool bInShouldApplyChangeImmediately)
	{
		bShouldApplyChangeImmediately = bInShouldApplyChangeImmediately;
	}
};
