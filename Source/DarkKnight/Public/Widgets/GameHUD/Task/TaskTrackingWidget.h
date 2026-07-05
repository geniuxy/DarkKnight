// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "TaskTrackingWidget.generated.h"

class ADkPlayerStateBase;
class ADkCharacterHero;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UTaskTrackingWidget : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	//~Begin UUserWidget UFunction
	virtual void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	//~End UUserWidget UFunction

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonTextBlock* TaskNameText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonTextBlock* TaskContentText;
	//***** Bound Widgets *****//

	UPROPERTY()
	TObjectPtr<ADkCharacterHero> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<ADkPlayerStateBase> OwnerPlayerState;

	void ConfigureTaskTrackingInfo(const FText& InTaskName, const FText& InTaskContent, bool bForce);
	void ClearTaskTrackingInfo();
};
