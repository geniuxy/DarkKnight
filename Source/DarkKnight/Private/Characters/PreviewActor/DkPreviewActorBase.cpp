// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PreviewActor/DkPreviewActorBase.h"

#include "Components/DkEquipmentComponent.h"


ADkPreviewActorBase::ADkPreviewActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());

	EquipmentComponent = CreateDefaultSubobject<UDkEquipmentComponent>("EquipmentComponent");
	EquipmentComponent->SetOwningSkeletalMesh(Mesh);
	EquipmentComponent->SetIsPreview(true);
}

void ADkPreviewActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

