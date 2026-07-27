// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment/DkEquippedActorBase.h"

#include "Net/UnrealNetwork.h"


ADkEquippedActorBase::ADkEquippedActorBase()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
}

void ADkEquippedActorBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ItemId);
}

void ADkEquippedActorBase::SetItemId(int InItemId)
{
	// 对于玩家角色，是Server端执行；
	// 对于PreviewActor，是Client端执行。DOREPLIFETIME 只建立 Server → Client 的单向通道，所以不会触发OnRep_ItemId
	ItemId = InItemId;

	UpdateEquipmentItemInfo();
}

void ADkEquippedActorBase::OnRep_ItemId()
{
	UpdateEquipmentItemInfo();
}

void ADkEquippedActorBase::UpdateEquipmentItemInfo()
{
}


