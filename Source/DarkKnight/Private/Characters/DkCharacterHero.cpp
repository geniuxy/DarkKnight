// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DkCharacterHero.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DkEquipmentComponent.h"
#include "Components/DkInventoryComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GAS/DkAbilitySystemComponent.h"
#include "PlayerStates/DkPlayerStateBase.h"


class ADkPlayerStateBase;

ADkCharacterHero::ADkCharacterHero()
{
	PrimaryActorTick.bCanEverTick = false;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 200.f;
	CameraBoom->SocketOffset = FVector(0.f, 80.f, 50.f);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	InventoryComponent = CreateDefaultSubobject<UDkInventoryComponent>(TEXT("DkInventoryComponent"));
	EquipmentComponent = CreateDefaultSubobject<UDkEquipmentComponent>(TEXT("DkEquipmentComponent"));

	LandedDelegate.AddDynamic(this, &ThisClass::HandleOnLanded);
}

void ADkCharacterHero::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for the Server
	InitAbilityActorInfo();
}

void ADkCharacterHero::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();

	SwitchLocomotionStyle(ELocomotionStyle::Walk);
}

void ADkCharacterHero::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	if (PrevMovementMode == MOVE_Walking && GetCharacterMovement()->MovementMode == MOVE_Falling) // TODO: 并且没有骑上马
	{
		LastJumpStartPosition = GetActorLocation();
	}
}

void ADkCharacterHero::InitAbilityActorInfo()
{
	ADkPlayerStateBase* OwningPlayerState = GetPlayerState<ADkPlayerStateBase>();
	check(OwningPlayerState);
	OwningPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(OwningPlayerState, this);
	OwningPlayerState->GetAbilitySystemComponent()->AbilityActorInfoSet();
	AbilitySystemComponent = OwningPlayerState->GetAbilitySystemComponent();
	AttributeSet = OwningPlayerState->GetAttributeSet();
}

void ADkCharacterHero::HandleOnLanded(const FHitResult& Hit)
{
	// 跳跃高度过高
	if (LastJumpStartPosition.Z - GetActorLocation().Z >= DeadlyFallHeight)
	{
		// 彻底禁用角色移动组件
		GetCharacterMovement()->DisableMovement();
		GetCharacterMovement()->StopMovementImmediately();

		// 关闭胶囊体的碰撞，防止它和Mesh冲突
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 设置Mesh物理
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		if (BodyArmorMesh) BodyArmorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (ArmArmorMesh) ArmArmorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (BottomArmorMesh) BottomArmorMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		if (HelmetMesh) HelmetMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		GetMesh()->SetSimulatePhysics(true);

		// 解除Mesh和胶囊体的绑定，否则Mesh会被胶囊体"拖着走"
		GetMesh()->DetachFromComponent(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));

		// 可选：给Mesh一个随机的微小旋转/冲量，让死亡更自然
		// GetMesh()->AddImpulse(FVector(FMath::RandRange(-100, 100), FMath::RandRange(-100, 100), 0));

		// TODO: 这里死了，会到处飘，还是播放死亡动画吧，受不了了，不会改
	}
}
