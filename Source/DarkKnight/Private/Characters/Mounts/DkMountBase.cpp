// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mounts/DkMountBase.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Characters/DkCharacterBase.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/DkAbilitySystemComponent.h"
#include "GAS/DkAttributeSet.h"
#include "Kismet/KismetMathLibrary.h"


class UEnhancedInputLocalPlayerSubsystem;

ADkMountBase::ADkMountBase()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	GetCapsuleComponent()->SetCapsuleRadius(45.f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(90.f);

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
	Mane->SetLeaderPoseComponent(GetMesh());
	Tail->SetLeaderPoseComponent(GetMesh());
	Saddle->SetLeaderPoseComponent(GetMesh());
	Reins->SetLeaderPoseComponent(GetMesh());

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
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxAcceleration = 1500.f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->SetFixedBrakingDistance(200.f); // AI导航：让 NPC 在到达目标点前恰好停在 200cm 处，不会冲过头
	// 通常同角色的实际碰撞体，确保AI导航网格查询使用正确的角色体积
	GetCharacterMovement()->NavAgentProps.AgentRadius = GetCapsuleComponent()->GetUnscaledCapsuleRadius();
	GetCharacterMovement()->NavAgentProps.AgentHeight = GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight() * 2.f;
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

void ADkMountBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (LocomotionActualSpeed != LocomotionExpectedSpeed)
	{
		LocomotionActualSpeed = UKismetMathLibrary::FInterpTo(
			LocomotionActualSpeed, LocomotionExpectedSpeed, DeltaSeconds, LocomotionInterpSpeed
		);
	}

	if (IsLocallyControlledByPlayer())
	{
		UpdateCameraRotation(DeltaSeconds);
	}
}

bool ADkMountBase::IsLocallyControlledByPlayer()
{
	return GetController() && GetController()->IsLocalPlayerController();
}

ADkCharacterBase* ADkMountBase::GetOwnerInstigator()
{
	if (!GetInstigator()) return nullptr;

	ADkCharacterBase* OwnerInstigator = Cast<ADkCharacterBase>(GetInstigator());
	return OwnerInstigator;
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

void ADkMountBase::UpdateCameraRotation(float DeltaSeconds)
{
	ADkCharacterBase* OwnerInstigator = GetOwnerInstigator();
	if (!OwnerInstigator && false) // TODO: 这个记得修改
	{
		return;
	}

	if (LocomotionExpectedSpeed >= 100.f)
	{
		DeltaRotator =
			UKismetMathLibrary::NormalizedDeltaRotator(GetBaseAimRotation(), GetActorRotation()).Yaw;
		if ((DeltaRotator >= 35.f && DeltaRotator <= 145.f) || (DeltaRotator >= -145.f && DeltaRotator <= -35.f))
		{
			StartCameraFaceForward(DeltaSeconds);
		}
	}
	else
	{
		bStartCameraLocateForward = false;
	}
}

void ADkMountBase::StartCameraFaceForward(float DeltaSeconds)
{
	if (bStartCameraLocateForward) return;

	bStartCameraLocateForward = true;
	GetWorldTimerManager().SetTimer(
		StartCameraFaceForwardHandle,
		this,
		&ThisClass::HandleCameraFaceForward,
		DeltaSeconds,
		true
	);
}

void ADkMountBase::HandleCameraFaceForward()
{
	float DeltaSeconds = GetWorld()->GetDeltaSeconds();
	if (!bTempStopCameraLocateForward)
	{
		double RotateSpeed = UKismetMathLibrary::MapRangeClamped(
			FMath::Abs(DeltaRotator), 0.f, 180.f, 2.f, 5.f
		);
		FRotator NewRotation = UKismetMathLibrary::RInterpTo(
			GetControlRotation(),
			UKismetMathLibrary::Conv_VectorToRotator(GetActorForwardVector() - FVector(0.f, 0.f, 0.35f)), // 微调方向
			DeltaSeconds,
			RotateSpeed
		);
		GetController()->SetControlRotation(NewRotation);
	}

	// 站着不动时会停止摄像机朝前
	if (!bStartCameraLocateForward)
	{
		GetWorldTimerManager().ClearTimer(StartCameraFaceForwardHandle);
	}
}

FVector ADkMountBase::GetDesiredMovement()
{
	FVector DesiredMovement;
	ADkCharacterBase* OwnerInstigator = GetOwnerInstigator();
	if (!OwnerInstigator && false) // TODO: 这个记得修改
	{
		DesiredMovement = UKismetMathLibrary::GetDirectionUnitVector(GetActorLocation(), AITargetLocation);
	}
	else
	{
		//FRotator()第一个是Pitch，第二个是Yaw，第三个是Roll，这个是根据显示器的xyz决定的，x左右，y上下，z垂直于显示器
		FVector ForwardDesiredMovement =
			UKismetMathLibrary::GetForwardVector(FRotator(0.f, GetControlRotation().Yaw, 0.f)) * MovementInputY;
		FVector RightDesiredMovement =
			UKismetMathLibrary::GetRightVector(FRotator(0.f, GetControlRotation().Yaw, 0.f)) * MovementInputX;
		DesiredMovement = ForwardDesiredMovement + RightDesiredMovement;
	}

	return DesiredMovement;
}

void ADkMountBase::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamID = NewTeamID;
}

FGenericTeamId ADkMountBase::GetGenericTeamId() const
{
	return TeamID;
}
