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
}

void ADkCharacterBase::InitializeCharacterInfo()
{
}

void ADkCharacterBase::InitAbilityActorInfo()
{
}

void ADkCharacterBase::SwitchLocomotionStyle(ELocomotionStyle InStyle)
{
	checkf(AbilitySystemComponent, TEXT("该Character类没有配置AbilitySystemComponent"));
	bool bFound = false;
	float MoveSpeedPercent =
		AbilitySystemComponent->GetGameplayAttributeValue(UDkAttributeSet::GetMoveSpeedAttribute(), bFound);
	if (!bFound)
	{
		Debug::Print("AttributeSet中没有配置MoveSpeed");
	}

	SetCurrentLocomotionStyle(InStyle);

	checkf(CharacterInfo, TEXT("该Character类没有配置CharacterInfo"));
	switch (InStyle)
	{
	case ELocomotionStyle::Walk:
		GetCharacterMovement()->MaxWalkSpeed = CharacterInfo->MaxWalkSpeed * MoveSpeedPercent;
		break;
	case ELocomotionStyle::Run:
		GetCharacterMovement()->MaxWalkSpeed = CharacterInfo->MaxRunSpeed * MoveSpeedPercent;
		break;
	case ELocomotionStyle::Sprint:
		GetCharacterMovement()->MaxWalkSpeed = CharacterInfo->MaxSprintSpeed * MoveSpeedPercent;
		break;
	case ELocomotionStyle::Unknown:
		break;
	default:
		break;
	}

	Debug::Print(TEXT("行走速度"), GetCharacterMovement()->MaxWalkSpeed);
}
