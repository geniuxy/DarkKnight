// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mounts/DkMountBase.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/DkAbilitySystemComponent.h"
#include "GAS/DkAttributeSet.h"


class UEnhancedInputLocalPlayerSubsystem;

ADkMountBase::ADkMountBase()
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
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = FVector(0.f, 80.f, 50.f);
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

	AbilitySystemComponent = CreateDefaultSubobject<UDkAbilitySystemComponent>(TEXT("Ability System Component"));
	AttributeSet = CreateDefaultSubobject<UDkAttributeSet>(TEXT("AttributeSet"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ADkMountBase::PawnClientRestart()
{
	Super::PawnClientRestart();

	APlayerController* OwningPlayerController = GetController<APlayerController>();
	if (OwningPlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
			OwningPlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (InputSubsystem)
		{
			InputSubsystem->RemoveMappingContext(AbilitySystemComponent->GetInputMappingContext());
			InputSubsystem->AddMappingContext(AbilitySystemComponent->GetInputMappingContext(), 0);
		}
	}
}

UAbilitySystemComponent* ADkMountBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ADkMountBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ADkMountBase::HandleAbilityInput(const FInputActionValue& InputActionValue, EAbilityInputID InputID)
{
}

void ADkMountBase::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamID = NewTeamID;
}

FGenericTeamId ADkMountBase::GetGenericTeamId() const
{
	return TeamID;
}
