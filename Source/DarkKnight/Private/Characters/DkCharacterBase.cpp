// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DkCharacterBase.h"

#include "DarkKnightDebugHelper.h"
#include "MotionWarpingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DkActionComponent.h"
#include "DarkKnight/DarkKnight.h"
#include "DataAssets/CharacterInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
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
	GetMesh()->ComponentTags.Add(FName("MainMesh"));

	GetCapsuleComponent()->SetCollisionProfileName(FName("DkMesh"));
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Weapon, ECR_Ignore);

	BodyArmorMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BodyArmorMesh");
	BodyArmorMesh->SetupAttachment(GetMesh());
	BodyArmorMesh->SetCollisionProfileName(FName("DkMesh"));
	BodyArmorMesh->ComponentTags.Add(FName("BodyArmorMesh"));
	ArmArmorMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ArmArmorMesh");
	ArmArmorMesh->SetupAttachment(GetMesh());
	ArmArmorMesh->SetCollisionProfileName(FName("DkMesh"));
	ArmArmorMesh->ComponentTags.Add(FName("ArmArmorMesh"));
	BottomArmorMesh = CreateDefaultSubobject<USkeletalMeshComponent>("BottomArmorMesh");
	BottomArmorMesh->SetupAttachment(GetMesh());
	BottomArmorMesh->SetCollisionProfileName(FName("DkMesh"));
	BottomArmorMesh->ComponentTags.Add(FName("BottomArmorMesh"));
	HelmetMesh = CreateDefaultSubobject<USkeletalMeshComponent>("HelmetMesh");
	HelmetMesh->SetupAttachment(GetMesh());
	HelmetMesh->SetCollisionProfileName(FName("DkMesh"));
	HelmetMesh->ComponentTags.Add(FName("HelmetMesh"));

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

	GetNetworkDebugInfo();
}

void ADkCharacterBase::InitializeCharacterInfo()
{
}

void ADkCharacterBase::InitAbilityActorInfo()
{
}

void ADkCharacterBase::ServerSpawnRewardItemActor_Implementation()
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

void ADkCharacterBase::GetNetworkDebugInfo() const
{
	if (!bDebugNetworkInfo) return; 
	
	// 1. NetMode (当前运行模式)
	FString NetModeStr;
	switch (GetNetMode())
	{
		case NM_Standalone:    NetModeStr = TEXT("Standalone"); break;
		case NM_DedicatedServer: NetModeStr = TEXT("DedicatedServer"); break;
		case NM_ListenServer:  NetModeStr = TEXT("ListenServer"); break;
		case NM_Client:        NetModeStr = TEXT("Client"); break;
		default:               NetModeStr = TEXT("Unknown"); break;
	}
	
	// 2. NetRole (本端的角色)
	FString RoleStr;
	switch (GetLocalRole())
	{
		case ROLE_None:            RoleStr = TEXT("None"); break;
		case ROLE_SimulatedProxy:  RoleStr = TEXT("SimulatedProxy"); break;
		case ROLE_AutonomousProxy: RoleStr = TEXT("AutonomousProxy"); break;
		case ROLE_Authority:       RoleStr = TEXT("Authority"); break;
		default:                   RoleStr = TEXT("Unknown"); break;
	}
	
	// 3. RemoteRole (对端的角色)
	FString RemoteRoleStr;
	switch (GetRemoteRole())
	{
		case ROLE_None:            RemoteRoleStr = TEXT("None"); break;
		case ROLE_SimulatedProxy:  RemoteRoleStr = TEXT("SimulatedProxy"); break;
		case ROLE_AutonomousProxy: RemoteRoleStr = TEXT("AutonomousProxy"); break;
		case ROLE_Authority:       RemoteRoleStr = TEXT("Authority"); break;
		default:                   RemoteRoleStr = TEXT("Unknown"); break;
	}
	
	// 4. 关键判断
	bool bHasAuthority = HasAuthority();
	bool bIsLocallyControlled = IsLocallyControlled();
	bool bIsServer = (GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer);
	bool bIsClientOnly = (GetNetMode() == NM_Client);
	bool bIsStandalone = (GetNetMode() == NM_Standalone);
	
	// 组装信息
	const FString Info = FString::Printf(
		TEXT("=== %s ===\n")
		TEXT("NetMode: %s\n")
		TEXT("LocalRole: %s | RemoteRole: %s\n")
		TEXT("HasAuthority: %s\n")
		TEXT("IsLocallyControlled: %s\n")
		TEXT("IsServer: %s | IsClientOnly: %s | IsStandalone: %s\n")
		TEXT("Controller: %s\n")
		TEXT("PlayerState: %s"),
		*GetName(),
		*NetModeStr,
		*RoleStr,
		*RemoteRoleStr,
		bHasAuthority ? TEXT("true") : TEXT("false"),
		bIsLocallyControlled ? TEXT("true") : TEXT("false"),
		bIsServer ? TEXT("true") : TEXT("false"),
		bIsClientOnly ? TEXT("true") : TEXT("false"),
		bIsStandalone ? TEXT("true") : TEXT("false"),
		GetController() ? *GetController()->GetName() : TEXT("NULL"),
		GetPlayerState() ? *GetPlayerState()->GetPlayerName() : TEXT("NULL")
	);
	
	Debug::Print(Info, -1, 30.f);
}
