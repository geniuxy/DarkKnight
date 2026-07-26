// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUp/DkPickUpActorStaticMesh.h"

#include "DarkKnightDebugHelper.h"
#include "Components/DkHighlightableStaticMesh.h"
#include "DkTypes/DkStructs.h"


ADkPickUpActorStaticMesh::ADkPickUpActorStaticMesh()
{
	PrimaryActorTick.bCanEverTick = false;

	PickUpItemStaticMesh = CreateDefaultSubobject<UDkHighlightableStaticMesh>(TEXT("PickUpItemStaticMesh"));
	PickUpItemStaticMesh->SetIsReplicated(true);
	SetRootComponent(PickUpItemStaticMesh);
}

void ADkPickUpActorStaticMesh::SetPickUpItemInfo(const FDkItemInfo& PickUpItemInfo, int32 InItemStack)
{
	Super::SetPickUpItemInfo(PickUpItemInfo, InItemStack);

	SetItemStaticMesh(PickUpItemInfo);
}

void ADkPickUpActorStaticMesh::SetItemStaticMesh(const FDkItemInfo& PickUpItemInfo)
{
	if (PickUpItemInfo.bStaticMesh)
	{
		if (PickUpItemInfo.ItemStaticMesh.IsValid())
		{
			PickUpItemStaticMesh->SetStaticMesh(PickUpItemInfo.ItemStaticMesh.LoadSynchronous());
		}
		else
		{
			Debug::Print(TEXT("生成PickUpItem时，表格配置是StaticMesh，却没有配置StaticMesh"));
		}
	}
}
