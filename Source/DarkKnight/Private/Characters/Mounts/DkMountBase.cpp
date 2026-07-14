// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Mounts/DkMountBase.h"

#include "EnhancedInputSubsystems.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
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

void ADkMountBase::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->SetMovementMode(MOVE_Walking);

	// TryToCalculateAITargetLocation();
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

	if (HasAuthority())
	{
		// TODO: 用 AIController + MoveTo 替代手动寻路，并且马的动画RootMotion的话不适用于AIController, 马的动画不能用RootMotion
		UpdateAIMovement(DeltaSeconds);
	}
}

bool ADkMountBase::IsLocallyControlledByPlayer()
{
	return GetController() && GetController()->IsLocalPlayerController();
}

ADkCharacterBase* ADkMountBase::GetOwnerInstigator() const
{
	return OwnerInstigator;
}

void ADkMountBase::SetOwnerInstigator(ADkCharacterBase* InInstigator)
{
	SetInstigator(InInstigator);
	OwnerInstigator = InInstigator;
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
	if (!IsRiderOn && false) // TODO: 这个记得修改
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
	if (!IsRiderOn)
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

void ADkMountBase::UpdateAIMovement(float DeltaSeconds)
{
	if (IsRiderOn) 
	{
		StopAIMovement();
		return;
	}
    
	if (!OwnerInstigator)
	{
		StopAIMovement();
		return;
	}
    
	float DistToOwner = GetHorizontalDistanceTo(OwnerInstigator);
    
	// 足够近，停止跟随
	if (DistToOwner <= StopFollowDistance)  // 300.f
	{
		StopAIMovement();
		return;
	}
    
	// 需要跟随，但还没开始
	if (!IsAIMoving)
	{
		StartAIMovement();
	}
    
	// 更新目标点（降低频率，不依赖Timer）
	UpdateFollowTarget(DeltaSeconds);
    
	// 根据距离平滑选择速度
	UpdateLocomotionByDistance(DistToOwner);
    
	// 检查是否到达当前目标点
	CheckArrivalAtTarget();
}

void ADkMountBase::StopAIMovement()
{
	if (!IsAIMoving) return;
    
	IsAIMoving = false;
	SetMoveType(EMountMoveType::Idle);
	SetLocomotionExpectedSpeed(0.f);
	SetLocomotionInterpSpeed(85.f);
	GetWorldTimerManager().ClearTimer(AIMovementTimerHandle);
}

void ADkMountBase::StartAIMovement()
{
	IsAIMoving = true;
	// 初始给一个较高速度快速接近
	SetLocomotionExpectedSpeed(650.f);
	SetLocomotionInterpSpeed(400.f);
}

void ADkMountBase::UpdateFollowTarget(float DeltaSeconds)
{
	TargetUpdateCooldown -= DeltaSeconds;
	if (TargetUpdateCooldown > 0.f) return;
    
	TargetUpdateCooldown = 0.5f;  // 每0.5秒更新一次目标
    
	if (!OwnerInstigator) return;
    
	FVector OwnerLoc = OwnerInstigator->GetActorLocation();
    
	// 固定偏移：在主人后方一定距离，带轻微随机
	FVector DesiredOffset = OwnerInstigator->GetActorForwardVector() * -150.f  // 后方150
						  + OwnerInstigator->GetActorRightVector() * FMath::FRandRange(-80.f, 80.f);
    
	FVector TargetLoc = OwnerLoc + DesiredOffset;
    
	// 寻路
	UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(
		this, GetActorLocation(), TargetLoc);
    
	if (Path && Path->PathPoints.Num() > 0)
	{
		// 平滑目标点变化，避免跳动
		FVector NewTarget = Path->PathPoints.Num() > 1 ? Path->PathPoints[1] : Path->PathPoints[0];
		AITargetLocation = FMath::Lerp(AITargetLocation, NewTarget, 0.3f);  // 平滑过渡
	}
}

void ADkMountBase::UpdateLocomotionByDistance(float DistanceToOwner)
{
	float T = FMath::Clamp((DistanceToOwner - 300.f) / 700.f, 0.f, 1.f);
	float EasedT = FMath::InterpEaseInOut(0.f, 1.f, T, 2.f);  // 缓动曲线
    
	float TargetSpeed = EasedT * 650.f;
	float InterpSpeed = FMath::Lerp(85.f, 400.f, EasedT);
    
	// 根据速度选择移动类型
	EMountMoveType NewType = EMountMoveType::Idle;
	if (TargetSpeed > 500.f)      NewType = EMountMoveType::Canter;
	else if (TargetSpeed > 200.f) NewType = EMountMoveType::Trot;
	else if (TargetSpeed > 10.f)  NewType = EMountMoveType::Walk;
    
	SetMoveType(NewType);
	SetLocomotionExpectedSpeed(TargetSpeed);
	SetLocomotionInterpSpeed(InterpSpeed);
}

void ADkMountBase::CheckArrivalAtTarget()
{
	float DistToTarget = UKismetMathLibrary::Vector_Distance2D(GetActorLocation(), AITargetLocation);
	if (DistToTarget <= 100.f)
	{
		// 不停止移动，只是触发一次目标点刷新
		// 让 UpdateFollowTarget 在下次冷却到期时计算新目标
		TargetUpdateCooldown = 0.f;  // 强制立即刷新目标
	}
}

void ADkMountBase::MoveToAITargetLocation()
{
	if (IsRiderOn) return;
	if (!IsAIMoving)
	{
		TryToCalculateAITargetLocation();
		return;
	}

	float DistanceToTarget = UKismetMathLibrary::Vector_Distance2D(GetActorLocation(), AITargetLocation);
	if (DistanceToTarget <= 300.f)
	{
		SetMoveType(EMountMoveType::Idle);
		SetLocomotionExpectedSpeed(0.f);
		SetLocomotionInterpSpeed(85.f);
		IsAIMoving = false;
		GetWorldTimerManager().ClearTimer(AIMovementTimerHandle);
	}
	else if (DistanceToTarget <= 600.f)
	{
		SetMoveType(EMountMoveType::Walk);
		SetLocomotionExpectedSpeed(100.f);
		SetLocomotionInterpSpeed(200.f);
	}
	else if (DistanceToTarget <= 1000.f)
	{
		SetMoveType(EMountMoveType::Trot);
		SetLocomotionExpectedSpeed(350.f);
		SetLocomotionInterpSpeed(200.f);
	}
	else
	{
		SetMoveType(EMountMoveType::Canter);
		SetLocomotionExpectedSpeed(650.f);
		SetLocomotionInterpSpeed(400.f);
	}
}

void ADkMountBase::TryToCalculateAITargetLocation()
{
	if (!OwnerInstigator) return;
	if (GetHorizontalDistanceTo(OwnerInstigator) <= 300.f)
	{
		IsAIMoving = false;
		return;
	}

	IsAIMoving = true;
	SetMoveType(EMountMoveType::Canter);
	SetLocomotionExpectedSpeed(650.f);
	SetLocomotionInterpSpeed(400.f);

	GetWorldTimerManager().SetTimer(
		AIMovementTimerHandle,
		this,
		&ThisClass::CalculateAITargetLocation,
		1.f,
		true,
		0.f
	);
}

void ADkMountBase::CalculateAITargetLocation()
{
	if (!OwnerInstigator) return;
	FVector OwnerInstigatorLocation = OwnerInstigator->GetActorLocation();
	FVector MountLocation = GetActorLocation();
	FVector LeftRightOffset = GetActorRightVector() * FMath::FRandRange(-100.f, 100.f);
	FVector FwdBwdOffset = GetActorForwardVector() * FMath::FRandRange(-200.f, -50.f);
	FVector TargetLocation = OwnerInstigatorLocation + LeftRightOffset + FwdBwdOffset;

	UNavigationPath* MountToTargetPath = UNavigationSystemV1::FindPathToLocationSynchronously(
		this, MountLocation, TargetLocation
	);
	if (!MountToTargetPath) return;
	AITargetLocation =
		MountToTargetPath->PathPoints.Num() > 1 ? MountToTargetPath->PathPoints[1] : MountToTargetPath->PathPoints[0];
	DrawDebugSphere(GetWorld(), AITargetLocation, 5.f, 8, FColor::Green, false, 10.f);
}

void ADkMountBase::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamID = NewTeamID;
}

FGenericTeamId ADkMountBase::GetGenericTeamId() const
{
	return TeamID;
}
