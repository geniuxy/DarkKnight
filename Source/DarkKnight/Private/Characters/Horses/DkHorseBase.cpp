// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Horses/DkHorseBase.h"

#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"


ADkHorseBase::ADkHorseBase()
{
	PrimaryActorTick.bCanEverTick = false;

	PushCrowdBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PushCrowdBox"));
	PushCrowdBox->SetupAttachment(GetRootComponent());
	InteractionWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractionWidget"));
	InteractionWidget->SetupAttachment(GetRootComponent());
	MountSphere = CreateDefaultSubobject<USphereComponent>(TEXT("MountSphere"));
	MountSphere->SetupAttachment(GetRootComponent());

	Mane = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mane"));
	Tail = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Tail"));
	Saddle = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Saddle"));
	Reins = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Reins"));
	Mane->SetupAttachment(GetMesh());
	Tail->SetupAttachment(GetMesh());
	Saddle->SetupAttachment(GetMesh());
	Reins->SetupAttachment(GetMesh());

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 170.f;
	CameraBoom->SocketOffset = FVector(0.f, 60.f, 0.f);
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom);
	FollowCamera->bUsePawnControlRotation = false;

	MountPoint_FrontR = CreateDefaultSubobject<USceneComponent>(TEXT("MountPoint_FrontR"));
	MountPoint_FrontL = CreateDefaultSubobject<USceneComponent>(TEXT("MountPoint_FrontL"));
	MountPoint_R = CreateDefaultSubobject<USceneComponent>(TEXT("MountPoint_R"));
	MountPoint_L = CreateDefaultSubobject<USceneComponent>(TEXT("MountPoint_L"));
	MountPoint_B = CreateDefaultSubobject<USceneComponent>(TEXT("MountPoint_B"));
	CameraOriginalLocation = CreateDefaultSubobject<USceneComponent>(TEXT("CameraOriginalLocation"));
	MountPoint_FrontR->SetupAttachment(GetMesh());
	MountPoint_FrontL->SetupAttachment(GetMesh());
	MountPoint_R->SetupAttachment(GetMesh());
	MountPoint_L->SetupAttachment(GetMesh());
	MountPoint_B->SetupAttachment(GetMesh());
	CameraOriginalLocation->SetupAttachment(GetMesh());

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ADkHorseBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
