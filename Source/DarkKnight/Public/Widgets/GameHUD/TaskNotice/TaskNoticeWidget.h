// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "DkTypes/DkStructs.h"
#include "TaskNoticeWidget.generated.h"

class ADkPlayerStateBase;
class ADkGamePlayerController;
class ADkCharacterHero;
class UCommonTextBlock;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UTaskNoticeWidget : public UCommonUserWidget
{
	GENERATED_BODY()

public:
	void AddTaskNotice(ETaskNoticeState InTaskNoticeState, const FText& InTaskName);

protected:
	//~Begin UUserWidget UFunction
	virtual void NativeOnInitialized() override;
	virtual void OnAnimationStarted_Implementation(const UWidgetAnimation* Animation) override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;
	//~End UUserWidget UFunction

private:
	//***** Bound Widgets *****//
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonTextBlock* TaskNameText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget, AllowPrivateAccess="true"))
	UCommonTextBlock* TaskNoticeStateText;

	UPROPERTY(Transient, meta=(BindWidgetAnim))
	UWidgetAnimation* ShowNoticeAnimation;
	//***** Bound Widgets *****//

	UPROPERTY(EditDefaultsOnly, Category="Notice Sound")
	USoundBase* NewTaskSound;

	UPROPERTY(EditDefaultsOnly, Category="Notice Sound")
	USoundBase* TaskUpdateSound;

	UPROPERTY(EditDefaultsOnly, Category="Notice Sound")
	USoundBase* TaskCompletedSound;

	UPROPERTY(Transient)
	TArray<FTaskNoticeInfo> CachedTaskNoticeList;

	UPROPERTY()
	TObjectPtr<ADkCharacterHero> OwnerCharacter;

	UPROPERTY()
	TObjectPtr<ADkGamePlayerController> OwnerPlayerController;

	UPROPERTY()
	TObjectPtr<ADkPlayerStateBase> OwnerPlayerState;

	FTimerHandle TaskNoticeTimerHandle;
	void TryPlayTaskNotice();

	bool bIsPlayingNotice;
};
