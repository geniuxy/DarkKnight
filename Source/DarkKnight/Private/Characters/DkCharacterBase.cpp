// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DkCharacterBase.h"

#include "DarkKnightDebugHelper.h"
#include "MotionWarpingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DkActionComponent.h"
#include "DarkKnight/DarkKnight.h"
#include "DataAssets/CharacterInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GAS/DkAbilitySystemComponent.h"
#include "GAS/DkAttributeSet.h"
#include "PickUp/DkPickUpActorBase.h"
#include "PickUp/DkPickUpActorSkeletalMesh.h"
#include "PickUp/DkPickUpActorStaticMesh.h"
#include "Subsytems/DkInventorySubsystem.h"

ADkCharacterBase::ADkCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false; // 操作角色的面朝方向不会跟着控制器“走”
	bUseControllerRotationRoll = false;

	GetMesh()->SetCollisionProfileName(FName("DkMesh"));

	GetCapsuleComponent()->SetCollisionProfileName(FName("DkMesh"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Weapon, ECR_Ignore);

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

	GetCharacterMovement()->GravityScale = 2.5f;
	GetCharacterMovement()->MaxAcceleration = 800.f;
	GetCharacterMovement()->bUseSeparateBrakingFriction = true;
	GetCharacterMovement()->MaxWalkSpeed = 170.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 800.f; // 行走减速度
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->JumpOffJumpZFactor = 0.f; // 踩在头上不会“弹开”，不会造成冲击力
	GetCharacterMovement()->AirControl = 1.f; // 在空中能完全控制自己
	GetCharacterMovement()->RotationRate = FRotator(0.f, 300.f, 0.f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->PushForceFactor = 500.f;
	GetCharacterMovement()->RepulsionForce = 0.f;
	GetCharacterMovement()->NavAgentProps.AgentRadius = 42.f;
	GetCharacterMovement()->NavAgentProps.AgentHeight = 192.f; // 试图解决Ai的问题(但不知道是什么)

	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));

	ActionComponent = CreateDefaultSubobject<UDkActionComponent>(TEXT("DkActionComponent"));
}

void ADkCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeCharacterInfo();

	if (ActionComponent && IsValid(CharacterInfo))
	{
		ActionComponent->InitializeActorComponent(CharacterInfo);
	}
}

void ADkCharacterBase::InitializeCharacterInfo()
{
}

void ADkCharacterBase::InitAbilityActorInfo()
{
}

void ADkCharacterBase::SpawnRewardItemActor()
{
	if (RewardItemIDList.IsEmpty()) return;

	int32 RandIndex = FMath::RandRange(0, RewardItemIDList.Num() - 1);
	int32 RewardItemID = RewardItemIDList[RandIndex].ItemID;
	int32 RewardItemStack = RewardItemIDList[RandIndex].Stack;
	checkf(RewardItemStack > 0, TEXT("生成RewardItem时，Stack数量为0！"))
	
	UDkInventorySubsystem* InventorySubsystem = UDkInventorySubsystem::Get(this);
	checkf(InventorySubsystem, TEXT("生成RewardItem时，InventorySubsystem为空！"));
	
	if (const FDkItemInfo* RewardItemInfo = InventorySubsystem->GetCachedItemTable().Find(RewardItemID))
	{
		FVector RotatedForward = GetActorForwardVector();
		RotatedForward =
			RotatedForward.RotateAngleAxis(FMath::FRandRange(SpawnAngleMin, SpawnAngleMax), FVector::UpVector);
		FVector SpawnLocation =
			GetActorLocation() + RotatedForward * FMath::FRandRange(SpawnDistanceMin, SpawnDistanceMax);
		SpawnLocation.Z -= RelativeSpawnElevation;
		const FRotator SpawnRotation = FRotator::ZeroRotator;
		ADkPickUpActorBase* SpawnActor = GetWorld()->SpawnActor<ADkPickUpActorBase>(
			RewardItemInfo->PickUpActorBPClass, SpawnLocation, SpawnRotation
		);

		if (IsValid(SpawnActor))
		{
			if (RewardItemInfo->bStaticMesh)
			{
				CastChecked<ADkPickUpActorStaticMesh>(SpawnActor)->SetPickUpItemInfo(RewardItemInfo, RewardItemStack);
			}
			else
			{
				CastChecked<ADkPickUpActorSkeletalMesh>(SpawnActor)->SetPickUpItemInfo(RewardItemInfo, RewardItemStack);
			}
		}
	}
}

void ADkCharacterBase::SpawnRewardItemActor(int ItemID)
{
}
