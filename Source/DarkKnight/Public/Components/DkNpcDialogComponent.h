// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DkNpcDialogComponent.generated.h"


struct FDialogNpcDetail;
class UGameplayEffect;
class UDkWidgetDialogScreen;
class ADkGamePlayerController;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTriggerNextDialog, int /* NextDialogId */);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DARKKNIGHT_API UDkNpcDialogComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDkNpcDialogComponent();

	bool CanStartDialog();
	void TryStartDialog(ADkGamePlayerController* PC);

	void CacheNpcTransform(TMap<int, FDialogNpcDetail> InNpcInfos);
	void UpdateNpcTransform(TMap<int, FDialogNpcDetail> InNpcInfos);
	void ResetNpcTransform();

	FOnTriggerNextDialog OnTriggerNextDialog;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category="Interact")
	FString InteractMessage;

	UPROPERTY(EditAnywhere, Category="Dialog Details")
	int StartDialogId = 0;

	UPROPERTY(EditAnywhere, Category="Dialog Details")
	int NpcId = 0;

	UPROPERTY(EditAnywhere, Category="Dialog Details")
	float RelativeDistance = 250.f;

	UPROPERTY()
	TObjectPtr<UDkWidgetDialogScreen> CachedDialogScreen;

	UPROPERTY()
	TMap<int, FTransform> CachedNpcTransforms;

public:
	FORCEINLINE FString GetInteractMessage() const { return InteractMessage; }
	FORCEINLINE int GetNpcId() const { return NpcId; }
};
