// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "DkUITabListWidgetBase.generated.h"

class UDkUICommonButtonBase;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkUITabListWidgetBase : public UCommonTabListWidgetBase
{
	GENERATED_BODY()

public:
	void RequestRegisterTab(const FName& InTabID, const FText& InTabDisplayName);

private:
	//~ Begin UWidget Interface
#if WITH_EDITOR
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	//~ End UWidget Interface

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Tab List Settings",
		meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "10"))
	int32 DebugEditorPreviewTabCount = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI Tab List Settings",
		meta = (AllowPrivateAccess = "true", ClampMin = "1", ClampMax = "10"))
	TSubclassOf<UDkUICommonButtonBase> TabButtonEntryWidgetClass;
};
