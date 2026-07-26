// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DkPickUpActorBase.generated.h"

struct FDkItemInfo;
class UDkHighlightableSkeletalMesh;
class UDkItemComponent;

UCLASS()
class DARKKNIGHT_API ADkPickUpActorBase : public AActor
{
	GENERATED_BODY()

public:
	ADkPickUpActorBase();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void SetPickUpItemInfo(const FDkItemInfo& PickUpItemInfo, int32 InItemStack);

	UFUNCTION(Server, Reliable)
	void Server_SetItemId(int InItemId);
	
	UFUNCTION(Server, Reliable)
	void Server_SetItemStack(int InStackCount);

protected:
	virtual void BeginPlay() override;

	/* Actor Components */
	UPROPERTY(VisibleAnywhere)
	UDkItemComponent* ItemComponent;
	/*********/

	UPROPERTY(EditAnywhere, Category="Collections")
	bool IsCollectable = false; // 是否是Level本身就放置的可捡拾物

	UPROPERTY(EditAnywhere, Category="Collections")
	UTexture2D* CollectionIcon;

	void InitCollectionsInfo();
	void UpdatePickUpItemInfo();

	UPROPERTY(ReplicatedUsing = OnRep_ItemId)
	int32 ItemId = 0;

	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_ItemStack)
	int32 ItemStack = 0; // 0意味着不可堆叠，但数量为1个

	UFUNCTION()
	void OnRep_ItemId();
	
	UFUNCTION()
	void OnRep_ItemStack();

public:
	FORCEINLINE UTexture2D* GetCollectionIcon() const { return CollectionIcon; }
	FORCEINLINE bool GetIsCollectable() const { return IsCollectable; }
};
