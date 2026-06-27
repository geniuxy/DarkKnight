// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "DkWidgetDialogScreen.generated.h"

class UDkUIDialogSelectionButton;
class ADkCharacterHero;
class UDkPlayerDialogComponent;
struct FDialogContent;
class UVerticalBox;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkWidgetDialogScreen : public UDkWidgetActivatableBase
{
	GENERATED_BODY()

public:
	void UpdateDialogContent(FDialogContent CurrentDialogContent);

protected:
	//~Begin UUserWidget Function
	virtual void NativeOnInitialized() override;
	//~End UUserWidget Function

	//~ Begin UCommonActivatableWidget Function
	virtual void NativeOnActivated() override;
	virtual void NativeOnDeactivated() override;
	//~ End UCommonActivatableWidget Function

private:
	//~ Begin Bound Widgets
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonTextBlock* DialogContent;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UVerticalBox* SelectContentList;
	//~ End Bound Widgets

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDkUIDialogSelectionButton> DialogSelectionButtonClass;

	bool bCanClickToNextDialog = false;

	UPROPERTY()
	TObjectPtr<ADkCharacterHero> OwnerCharacter;
	
	UPROPERTY()
	TObjectPtr<UDkPlayerDialogComponent> OwnerDialogComponent;
};
