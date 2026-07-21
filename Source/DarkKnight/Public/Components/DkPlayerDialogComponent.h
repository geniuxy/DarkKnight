// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Widgets/Interact/DkWidgetDialogScreen.h"
#include "DkPlayerDialogComponent.generated.h"


struct FDialogNpcDetail;

DECLARE_MULTICAST_DELEGATE_TwoParams(
	FOnDialogBranchEventTriggered, int /* NpcId */, const FDialogBranchInfo& /* DialogBranchInfo */)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKKNIGHT_API UDkPlayerDialogComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDkPlayerDialogComponent();

	bool FindDialogGameplayTag(FGameplayTagContainer InTags);
	void AddDialogTag(FGameplayTag InTag);
	void UpdateCameraFocus(int CameraFocusNpcId, EDialogCameraType CameraType, const FTransform& CustomCameraTransform);
	void UseCustomCameraTransform(const FTransform& CustomCameraTransform);

	FOnDialogBranchEventTriggered OnDialogBranchEventTriggered;

protected:
	virtual void BeginPlay() override;

	void HandleDialogBranchEventTriggered(int InNpcId, const FDialogBranchInfo& InDialogBranchInfo);
	void HandleOpenShop(int InNpcId, const FDialogBranchInfo& InDialogBranchInfo);

private:
	FGameplayTagContainer CachedDialogTags;

	UPROPERTY()
	APlayerController* OwnerPC;

	UPROPERTY()
	ADkCharacterHero* CachedOwner;

	UPROPERTY()
	ACameraActor* CachedCameraActor;

	UPROPERTY(EditDefaultsOnly, Category="Camera")
	TMap<EDialogCameraType, FTransform> DialogCameraTransformMap;
};
