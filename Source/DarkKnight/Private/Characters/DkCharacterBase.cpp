// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DkCharacterBase.h"
#include "MotionWarpingComponent.h"

ADkCharacterBase::ADkCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	GetMesh()->SetCollisionProfileName(FName("DkMesh"));
	
	BodyArmorMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BodyArmorMesh");
	BodyArmorMesh->SetupAttachment(GetMesh());
	BodyArmorMesh->SetCollisionProfileName(FName("DkMesh"));
	ArmArmorMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ArmArmorMesh");
	ArmArmorMesh->SetupAttachment(GetMesh());
	ArmArmorMesh->SetCollisionProfileName(FName("DkMesh"));
	BottomArmorMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BottomArmorMesh");
	BottomArmorMesh->SetupAttachment(GetMesh());
	BottomArmorMesh->SetCollisionProfileName(FName("DkMesh"));
	HelmetMesh = CreateDefaultSubobject<USkeletalMeshComponent>("HelmetMesh");
	HelmetMesh->SetupAttachment(GetMesh());
	HelmetMesh->SetCollisionProfileName(FName("DkMesh"));

	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
}

