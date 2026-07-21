// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkTypes/DkStructs.h"
#include "Widgets/Components/Buttons/DkUICommonButtonImage.h"
#include "DkUIDialogSelectionButton.generated.h"

class UDkPlayerDialogComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSelectionButtonClicked, int /* NextDialogId */);
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkUIDialogSelectionButton : public UDkUICommonButtonImage
{
	GENERATED_BODY()

public:
	void ConfigureDialogSelectionButton(
		const FDialogBranchInfo& InBranchInfo, UDkPlayerDialogComponent* InDialogComp, int InNpcId
	);

	//~Begin UCommonButtonBase UFunction
	virtual void NativeOnClicked() override;
	//~End UCommonButtonBase UFunction

	//~Begin UCommonButtonBase UFunction
	virtual void NativeOnHovered() override;
	virtual void NativeOnUnhovered() override;
	//~End UCommonButtonBase UFunction

	FOnSelectionButtonClicked OnSelectionButtonClicked;

private:
	bool HasFinishedBranch();
	
	UPROPERTY()
	TObjectPtr<UDkPlayerDialogComponent> OwnerDialogComponent;
	
	FDialogBranchInfo CurBranchInfo;

	int CurNpcId;
	
	UPROPERTY(EditAnywhere, Category="Branch Text")
	FLinearColor FinishedBranchTextColor = FLinearColor(0.4f,0.4f,0.4f,1.f);
};
