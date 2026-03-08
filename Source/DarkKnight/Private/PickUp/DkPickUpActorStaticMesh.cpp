// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp/DkPickUpActorStaticMesh.h"

#include "Components/DkHighlightableStaticMesh.h"
#include "DkTypes/DkStructs.h"


ADkPickUpActorStaticMesh::ADkPickUpActorStaticMesh()
{
	PrimaryActorTick.bCanEverTick = false;

	PickUpItemStaticMesh = CreateDefaultSubobject<UDkHighlightableStaticMesh>(TEXT("PickUpItemStaticMesh"));
}

void ADkPickUpActorStaticMesh::SetPickUpItemInfo(const FDkItemInfo* PickUpItemInfo, int32 InItemStack)
{
	Super::SetPickUpItemInfo(PickUpItemInfo, InItemStack);

	if (PickUpItemInfo->bStaticMesh)
	{
		checkf(IsValid(PickUpItemInfo->ItemStaticMesh), TEXT("生成PickUpItem时，表格配置是StaticMesh，却没有配置StaticMesh"));
		PickUpItemStaticMesh->SetStaticMesh(PickUpItemInfo->ItemStaticMesh);
	}
}
