// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/DkCharacterBase.h"

#include "DarkKnightDebugHelper.h"
#include "MotionWarpingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/DkActionComponent.h"
#include "Components/WidgetComponent.h"
#include "DarkKnight/DarkKnight.h"
#include "DataAssets/CharacterInfo.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerState.h"
#include "GAS/DkAbilitySystemComponent.h"
#include "GAS/DkAttributeSet.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "PickUp/DkPickUpActorBase.h"
#include "PickUp/DkPickUpActorSkeletalMesh.h"
#include "PickUp/DkPickUpActorStaticMesh.h"
#include "Subsytems/EngineSubsystems/DkDataSubsystem.h"
#include "Subsytems/EngineSubsystems/DkInventorySubsystem.h"
#include "Widgets/GameHUD/Stats/OverHeadStatsGauge.h"

ADkCharacterBase::ADkCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

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
	GetCharacterMovement()->MaxWalkSpeed = 170.f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->JumpOffJumpZFactor = 0.f; // 踩在头上不会“弹开”，不会造成冲击力
	GetCharacterMovement()->RotationRate = FRotator(0.f, 300.f, 0.f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->NavAgentProps.AgentRadius = 42.f;
	GetCharacterMovement()->NavAgentProps.AgentHeight = 192.f; // 试图解决Ai的问题(但不知道是什么)

	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));

	ActionComponent = CreateDefaultSubobject<UDkActionComponent>(TEXT("DkActionComponent"));

	AbilitySystemComponent = CreateDefaultSubobject<UDkAbilitySystemComponent>(TEXT("Ability System Component"));
	AttributeSet = CreateDefaultSubobject<UDkAttributeSet>(TEXT("AttributeSet"));

	OverHeadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Over Head Widget Component"));
	OverHeadWidgetComponent->SetupAttachment(GetRootComponent());
}

void ADkCharacterBase::ServerSideInit()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	AbilitySystemComponent->ServerSideInit();
}

void ADkCharacterBase::ClientSideInit()
{
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

bool ADkCharacterBase::IsLocallyControlledByPlayer()
{
	return GetController() && GetController()->IsLocalPlayerController();
}

void ADkCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeCharacterInfo();

	if (ActionComponent && IsValid(CharacterInfo))
	{
		ActionComponent->InitializeActorComponent(CharacterInfo);
	}

	MeshRelativeTransform = GetMesh()->GetRelativeTransform();

	GetNetworkDebugInfo();

	BindGASChangeDelegates(); // 构造函数中调用虚函数不好，此时虚函数表还没构建完成，子类的重写不会被调用。

	ConfigureOverHeadStatsWidget(); // 头顶血条的显示

	FTimerHandle DelayInitCharacterHandle;
	GetWorld()->GetTimerManager().SetTimer(DelayInitCharacterHandle, FTimerDelegate::CreateLambda([this]()
	{
		UpdateTaskTrackingActor();
	}), 0.2f, false);
}

void ADkCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!bIsLockingTarget || !IsValid(AbilitySystemComponent->GetLockTarget())) return;

	FaceLockTarget(DeltaSeconds);
}

void ADkCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (NewController && !NewController->IsPlayerController()) //专用于AICharacter在Server上的初始化
	{
		ServerSideInit();
	}
}

void ADkCharacterBase::InitializeCharacterInfo()
{
}

void ADkCharacterBase::InitAbilityActorInfo()
{
}

void ADkCharacterBase::ServerSpawnRewardItemActor_Implementation()
{
	if (RewardItemIdList.IsEmpty()) return;

	int32 RandIndex = FMath::RandRange(0, RewardItemIdList.Num() - 1);
	int32 RewardItemID = RewardItemIdList[RandIndex].ItemID;
	int32 RewardItemStack = RewardItemIdList[RandIndex].Stack;
	checkf(RewardItemStack > 0, TEXT("生成RewardItem时，Stack数量为0！"))

	UDkInventorySubsystem* InventorySubsystem = UDkInventorySubsystem::Get();
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
			SpawnActor->Server_SetItemId(RewardItemID);
			SpawnActor->Server_SetItemStack(RewardItemStack);
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
	case NM_Standalone: NetModeStr = TEXT("Standalone");
		break;
	case NM_DedicatedServer: NetModeStr = TEXT("DedicatedServer");
		break;
	case NM_ListenServer: NetModeStr = TEXT("ListenServer");
		break;
	case NM_Client: NetModeStr = TEXT("Client");
		break;
	default: NetModeStr = TEXT("Unknown");
		break;
	}

	// 2. NetRole (本端的角色)
	FString RoleStr;
	switch (GetLocalRole())
	{
	case ROLE_None: RoleStr = TEXT("None");
		break;
	case ROLE_SimulatedProxy: RoleStr = TEXT("SimulatedProxy");
		break;
	case ROLE_AutonomousProxy: RoleStr = TEXT("AutonomousProxy");
		break;
	case ROLE_Authority: RoleStr = TEXT("Authority");
		break;
	default: RoleStr = TEXT("Unknown");
		break;
	}

	// 3. RemoteRole (对端的角色)
	FString RemoteRoleStr;
	switch (GetRemoteRole())
	{
	case ROLE_None: RemoteRoleStr = TEXT("None");
		break;
	case ROLE_SimulatedProxy: RemoteRoleStr = TEXT("SimulatedProxy");
		break;
	case ROLE_AutonomousProxy: RemoteRoleStr = TEXT("AutonomousProxy");
		break;
	case ROLE_Authority: RemoteRoleStr = TEXT("Authority");
		break;
	default: RemoteRoleStr = TEXT("Unknown");
		break;
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

void ADkCharacterBase::DisableCollisionToPawn()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

UAbilitySystemComponent* ADkCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ADkCharacterBase::BindGASChangeDelegates()
{
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RegisterGameplayTagEvent(DkGameplayTags::Dk_Stats_Dead).AddUObject(
			this, &ThisClass::DeadTagUpdated
		);

		AbilitySystemComponent->RegisterGameplayTagEvent(DkGameplayTags::Dk_Stats_LockingTarget).AddUObject(
			this, &ThisClass::LockingTargetTagUpdated
		);

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UDkAttributeSet::GetMoveSpeedAttribute()
		).AddUObject(
			this, &ThisClass::MoveSpeedUpdated
		);
	}
}

void ADkCharacterBase::DeadTagUpdated(FGameplayTag Tag, int NewCount)
{
	if (NewCount != 0)
	{
		StartDeathSequence();
	}
	else
	{
		Respawn();
	}
}

void ADkCharacterBase::LockingTargetTagUpdated(FGameplayTag Tag, int NewCount)
{
	bIsLockingTarget = NewCount > 0;
	OnLockingTargetStateChanged(bIsLockingTarget);
}

void ADkCharacterBase::OnLockingTargetStateChanged(bool InbIsLockingTarget)
{
	// 在子类中重写
}

void ADkCharacterBase::FaceLockTarget(float DeltaSeconds)
{
	AActor* LockTarget = AbilitySystemComponent->GetLockTarget();
	if (!IsValid(LockTarget)) return;

	float RotationSpeed = 10.0f;
	FVector LookEndPoint = LockTarget->GetActorLocation();
	FRotator ActorLookRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LookEndPoint);
	FRotator CurActorRotation = GetActorRotation();
	FRotator NewRotation = FMath::RInterpTo(CurActorRotation, ActorLookRotation, DeltaSeconds, RotationSpeed);
	SetActorRotation(FRotator(0.f, NewRotation.Yaw, 0.f));

	AController* MyController = GetController();
	if (MyController)
	{
		FRotator CurControllerRotation = MyController->GetControlRotation();
		FRotator NewControllerRotation =
			FMath::RInterpTo(CurControllerRotation, ActorLookRotation, DeltaSeconds, RotationSpeed);
		MyController->SetControlRotation(FRotator(0.f, NewControllerRotation.Yaw, 0.f));
	}
}

void ADkCharacterBase::MoveSpeedUpdated(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

bool ADkCharacterBase::IsDead() const
{
	return GetAbilitySystemComponent()->HasMatchingGameplayTag(DkGameplayTags::Dk_Stats_Dead);
}

void ADkCharacterBase::RespawnImmediately()
{
	if (HasAuthority())
	{
		GetAbilitySystemComponent()->RemoveActiveEffectsWithGrantedTags(
			FGameplayTagContainer(DkGameplayTags::Dk_Stats_Dead)
		);
	}
}

void ADkCharacterBase::DeathMontageFinished()
{
	if (IsDead()) // 防止小兵在DeathMontageFinished触发前复活而导致的bug
	{
		SetRagDollEnabled(true);
	}
}

void ADkCharacterBase::SetRagDollEnabled(bool bIsEnabled)
{
	if (bIsEnabled)
	{
		GetMesh()->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		GetMesh()->SetSimulatePhysics(true);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	}
	else
	{
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		GetMesh()->SetRelativeTransform(MeshRelativeTransform);
	}
}

void ADkCharacterBase::PlayDeathAnimation()
{
	if (DeathMontage)
	{
		float MontageDuration = PlayAnimMontage(DeathMontage);
		GetWorldTimerManager().SetTimer(
			DeathMontageTimerHandle,
			this,
			&ThisClass::DeathMontageFinished,
			MontageDuration + DeathMontageFinishTimeShift
		);
	}
}

void ADkCharacterBase::StartDeathSequence()
{
	OnDeath();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->CancelAllAbilities(); // 取消所有目前还激活着的Abilities
	}

	PlayDeathAnimation();
	SetStatusGaugeEnabled(false);
	GetCharacterMovement()->SetMovementMode(MOVE_None); // 设置为MOVE_None便不会移动了，不适合被击飞的情况
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// SetAIPerceptionStimuliSourceEnabled(false); // 死了以后就不会被AI感知到
}

void ADkCharacterBase::Respawn()
{
	OnRespawn();
	// SetAIPerceptionStimuliSourceEnabled(true);
	SetRagDollEnabled(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
	GetMesh()->GetAnimInstance()->StopAllMontages(0.f);
	SetStatusGaugeEnabled(true);

	if (HasAuthority() && GetController())
	{
		TWeakObjectPtr<AActor> StartSpot = GetController()->StartSpot;
		if (StartSpot.IsValid())
		{
			SetActorTransform(StartSpot->GetActorTransform());
		}
	}

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->ApplyFullStatsEffect();
	}
}

void ADkCharacterBase::OnDeath()
{
}

void ADkCharacterBase::OnRespawn()
{
}

void ADkCharacterBase::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamID = NewTeamID;
}

FGenericTeamId ADkCharacterBase::GetGenericTeamId() const
{
	return TeamID;
}

FGameplayTag ADkCharacterBase::GetTaskTrackingTag() const
{
	return TaskTrackingTag;
}

void ADkCharacterBase::SetTaskTrackingTag(FGameplayTag InTag)
{
	TaskTrackingTag = InTag;
}

void ADkCharacterBase::UpdateTaskTrackingActor()
{
	if (GetTaskTrackingTag().IsValid())
	{
		UDkDataSubsystem::Get()->UpdateTaskTrackingInfo(GetTaskTrackingTag(), this);
	}
}

void ADkCharacterBase::ConfigureOverHeadStatsWidget()
{
	if (!OverHeadWidgetComponent)
	{
		return;
	}

	if (IsLocallyControlledByPlayer())
	{
		OverHeadWidgetComponent->SetHiddenInGame(true);
		return;
	}

	UOverHeadStatsGauge* OverHeadStatsGauge = Cast<UOverHeadStatsGauge>(OverHeadWidgetComponent->GetUserWidgetObject());
	if (OverHeadStatsGauge)
	{
		OverHeadStatsGauge->ConfigureWithASC(GetAbilitySystemComponent());
		OverHeadWidgetComponent->SetHiddenInGame(true);
		GetWorldTimerManager().ClearTimer(HeadStatsGaugeVisibilityUpdateTimerHandle);
		GetWorldTimerManager().SetTimer(
			HeadStatsGaugeVisibilityUpdateTimerHandle, this, &ThisClass::UpdateHeadGaugeVisibility,
			HeadStatsGaugeVisibilityCheckUpdateGap, true, 0.f
		);
	}
}

void ADkCharacterBase::UpdateHeadGaugeVisibility()
{
	APawn* LocalPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (LocalPlayerPawn)
	{
		float DistSquared = FVector::DistSquared(GetActorLocation(), LocalPlayerPawn->GetActorLocation());
		OverHeadWidgetComponent->SetHiddenInGame(DistSquared > HeadStatsGaugeVisibilityRangeSquared);
	}
}

void ADkCharacterBase::SetStatusGaugeEnabled(bool bIsEnabled)
{
	GetWorldTimerManager().ClearTimer(HeadStatsGaugeVisibilityUpdateTimerHandle);

	if (bIsEnabled)
	{
		ConfigureOverHeadStatsWidget();
	}
	else
	{
		OverHeadWidgetComponent->SetHiddenInGame(true);
	}
}
