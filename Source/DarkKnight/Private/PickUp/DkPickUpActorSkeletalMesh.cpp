// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp/DkPickUpActorSkeletalMesh.h"

#include "DarkKnightDebugHelper.h"
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

	SetItemSkeletalMesh(PickUpItemInfo);
}

void ADkPickUpActorSkeletalMesh::SetItemSkeletalMesh(const FDkItemInfo* PickUpItemInfo)
{
	if (!PickUpItemInfo->bStaticMesh)
	{
		if (IsValid(PickUpItemInfo->ItemSkeletalMesh))
		{
			PickUpItemSkeletalMesh->SetSkeletalMeshAsset(PickUpItemInfo->ItemSkeletalMesh);
		}
		else
		{
			Debug::Print(TEXT("生成PickUpItem时，表格配置不是StaticMesh，却没有配置SkeletalMesh"));
		}
	}
}
