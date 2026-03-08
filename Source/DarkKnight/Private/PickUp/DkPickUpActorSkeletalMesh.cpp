// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp/DkPickUpActorSkeletalMesh.h"

#include "Components/DkHighlightableSkeletalMesh.h"
#include "DkTypes/DkStructs.h"


ADkPickUpActorSkeletalMesh::ADkPickUpActorSkeletalMesh()
{
	PrimaryActorTick.bCanEverTick = false;

	PickUpItemSkeletalMesh = CreateDefaultSubobject<UDkHighlightableSkeletalMesh>(TEXT("PickUpItemSkeletalMesh"));
}

void ADkPickUpActorSkeletalMesh::SetPickUpItemInfo(const FDkItemInfo* PickUpItemInfo, int32 InItemStack)
{
	Super::SetPickUpItemInfo(PickUpItemInfo, InItemStack);

	if (!PickUpItemInfo->bStaticMesh)
	{
		checkf(IsValid(PickUpItemInfo->ItemSkeletalMesh), TEXT("生成PickUpItem时，表格配置不是StaticMesh，却没有配置SkeletalMesh"));
		PickUpItemSkeletalMesh->SetSkeletalMeshAsset(PickUpItemInfo->ItemSkeletalMesh);
	}
}
