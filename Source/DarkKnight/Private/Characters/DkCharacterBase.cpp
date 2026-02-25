// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DkCharacterBase.h"
#include "MotionWarpingComponent.h"

ADkCharacterBase::ADkCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	CharacterMesh = CreateDefaultSubobject<USkeletalMeshComponent>("CharacterMesh");
	CharacterMesh->SetupAttachment(GetRootComponent());
	CharacterMesh->SetCollisionProfileName(FName("DkMesh"));
	
	BodyArmorMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BodyArmorMesh");
	BodyArmorMesh->SetupAttachment(CharacterMesh);
	BodyArmorMesh->SetCollisionProfileName(FName("DkMesh"));
	ArmArmorMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ArmArmorMesh");
	ArmArmorMesh->SetupAttachment(CharacterMesh);
	ArmArmorMesh->SetCollisionProfileName(FName("DkMesh"));
	BottomArmorMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BottomArmorMesh");
	BottomArmorMesh->SetupAttachment(CharacterMesh);
	BottomArmorMesh->SetCollisionProfileName(FName("DkMesh"));
	HelmetMesh = CreateDefaultSubobject<USkeletalMeshComponent>("HelmetMesh");
	HelmetMesh->SetupAttachment(CharacterMesh);
	HelmetMesh->SetCollisionProfileName(FName("DkMesh"));

	CharacterMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));
}

