// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "DkUICraftReferenceButton.generated.h"

class UDkCraftListData;
class UCommonListView;
class UBorder;
class UCommonTextBlock;
class UCommonLazyImage;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkUICraftReferenceButton : public UCommonButtonBase, public IUserObjectListEntry
{
	GENERATED_BODY()

public:
	//~Begin IUserObjectListEntry Interface
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	//~End IUserObjectListEntry Interface

protected:
	//~Begin UCommonButtonBase UFunction
	virtual void NativeOnClicked() override;
	//~End UCommonButtonBase UFunction

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonLazyImage* BackgroundImg;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UBorder* BorderImg;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonLazyImage* CraftItemIcon;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonTextBlock* CraftItemNameText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonTextBlock* CraftMessageText;
	//***** Bound Widgets *****//

	UPROPERTY(EditAnywhere, Category="Message Color")
	FLinearColor NotReadyMessageColor = FLinearColor(0.4f,0.4f,0.4f,1.f);
	
	UPROPERTY(EditAnywhere, Category="Message Color")
	FLinearColor ReadyMessageColor = FLinearColor(1.f,1.f, 1.f,1.f);

	UPROPERTY()
	UCommonListView* OwnerListView;

	UPROPERTY()
	UDkCraftListData* CraftListData;
};
