// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DkTypes/DkStructs.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "DkWidgetDialogScreen.generated.h"

class UDkUICommonButtonImage;
class ADkGamePlayerController;
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
	void BeginDialog(int InStartDialogId);

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
	UCommonTextBlock* DialogContentText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UVerticalBox* SelectionList;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UDkUICommonButtonImage* DialogConfirmButton;
	//~ End Bound Widgets

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDkUIDialogSelectionButton> DialogSelectionButtonClass;
	
	UPROPERTY()
	TObjectPtr<ADkCharacterHero> OwnerCharacter;
	
	UPROPERTY()
	TObjectPtr<UDkPlayerDialogComponent> OwnerDialogComponent;

	UPROPERTY()
	TObjectPtr<UAudioComponent> CachedAudioComponent;

	UPROPERTY()
	TObjectPtr<ADkGamePlayerController> OwnerPC;
	
	bool bCanClickToNextDialog = false;
	int CurDialogId = 0;
	FDialogContent CurDialogContent;
	
	void DialogConfirmButtonClicked();
	void UpdateDialogContent();
	FDialogContent GetDialogInfoById(int InDialogId);
	int GetNextDialogId();
	void EndDialog();
};
