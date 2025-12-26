// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonListView.h"
#include "DkUICommonListView.generated.h"

class UDkUIOptionListEntryDataMapping;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkUICommonListView : public UCommonListView
{
	GENERATED_BODY()

protected:
	//~Begin UCommonListView Function
	virtual UUserWidget& OnGenerateEntryWidgetInternal(
		UObject* Item,
		TSubclassOf<UUserWidget> DesiredEntryClass,
		const TSharedRef<STableViewBase>& OwnerTable
	) override;

	virtual bool OnIsSelectableOrNavigableInternal(UObject* FirstSelectedItem) override;
	//~End UCommonListView Function

private:
	//~ Begin UWidget Interface
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	//~ End UWidget Interface

	UPROPERTY(EditAnywhere, Category = "UI List View Settings")
	UDkUIOptionListEntryDataMapping* ListEntryDataMapping;

public:
	FORCEINLINE UDkUIOptionListEntryDataMapping* GetListEntryDataMapping() const { return ListEntryDataMapping; }
};
