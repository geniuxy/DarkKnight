// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PreviewActor/DkPreviewActorBase.h"

#include "Components/DkEquipmentComponent.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"


ADkPreviewActorBase::ADkPreviewActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(GetRootComponent());
	Mesh->ComponentTags.Add(FName("MainMesh"));
	
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

	InitPrimaryWeaponComponent();

	InitSecondaryWeaponComponent();

	EquipmentComponent = CreateDefaultSubobject<UDkEquipmentComponent>("EquipmentComponent");
	EquipmentComponent->SetOwningSkeletalMesh(Mesh);
	EquipmentComponent->SetIsPreview(true);
}

void ADkPreviewActorBase::BeginPlay()
{
	Super::BeginPlay();
	DelayedInitialization();
}

void ADkPreviewActorBase::DelayedInitializeOwner()
{
	UWorld* World = GetWorld();
	if (!IsValid(World))
	{
		DelayedInitialization();
		return;
	}

	// APlayerController* PlayerController = nullptr;
	// for (FConstPlayerControllerIterator ControllerIterator = World->GetPlayerControllerIterator();
	//      ControllerIterator; ++ControllerIterator)
	// {
	// 	if (IsValid(ControllerIterator->Get()) && ControllerIterator->Get()->IsLocalPlayerController())
	// 	{
	// 		PlayerController = ControllerIterator->Get();
	// 		break;
	// 	}
	// }
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!IsValid(PlayerController))
	{
		DelayedInitialization();
		return;
	}

	ACharacter* Character = Cast<ACharacter>(PlayerController->GetPawn());
	if (!IsValid(Character))
	{
		DelayedInitialization();
		return;
	}

	USkeletalMeshComponent* CharacterMesh = Character->GetMesh();
	if (!IsValid(CharacterMesh))
	{
		DelayedInitialization();
		return;
	}

	SourceMesh = CharacterMesh;
	Mesh->SetSkeletalMesh(SourceMesh->GetSkeletalMeshAsset());
	Mesh->SetAnimInstanceClass(SourceMesh->GetAnimInstance()->GetClass());

	EquipmentComponent->InitializeOwner(PlayerController);
}

void ADkPreviewActorBase::DelayedInitialization()
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &ThisClass::DelayedInitializeOwner);
	GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);
}

void ADkPreviewActorBase::InitPrimaryWeaponComponent()
{
	PrimaryWeaponStorePoint = CreateDefaultSubobject<USceneComponent>(TEXT("PrimaryWeaponStorePoint"));
	PrimaryWeaponStorePoint->SetupAttachment(GetMesh(), FName(TEXT("spine_03")));
	
	PrimaryWeaponSheathRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrimaryWeaponSheathRoot"));
	PrimaryWeaponSheathRoot->SetupAttachment(PrimaryWeaponStorePoint);
	
	PrimaryWeaponSheathTarget = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrimaryWeaponSheathTarget"));
	PrimaryWeaponSheathTarget->SetupAttachment(PrimaryWeaponStorePoint);
	PrimaryWeaponSheathTarget->ComponentTags.Add(FName("PrimaryWeaponSheathTarget"));
	PrimaryWeaponSheathTarget->SetSimulatePhysics(true); // 这个设置对于武器摇曳非常重要
	
	PrimaryPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PrimaryPhysicsConstraint"));
	PrimaryPhysicsConstraint->SetupAttachment(PrimaryWeaponSheathRoot);
	PrimaryPhysicsConstraint->SetConstrainedComponents(
		PrimaryWeaponSheathRoot, FName(), PrimaryWeaponSheathTarget, FName()
	);
	PrimaryPhysicsConstraint->ConstraintInstance.SetLinearLimits(LCM_Limited, LCM_Limited, LCM_Limited, 1.25);
	PrimaryPhysicsConstraint->SetAngularSwing1Limit(ACM_Limited, 7.f);
	PrimaryPhysicsConstraint->SetAngularSwing2Limit(ACM_Limited, 10.f);
	PrimaryPhysicsConstraint->SetAngularTwistLimit(ACM_Limited, 10.f);
	PrimaryPhysicsConstraint->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
	PrimaryPhysicsConstraint->SetAngularOrientationDrive(true, true);
	PrimaryPhysicsConstraint->SetAngularVelocityDriveTwistAndSwing(true, true);
}

void ADkPreviewActorBase::InitSecondaryWeaponComponent()
{
	SecondaryWeaponStorePoint = CreateDefaultSubobject<USceneComponent>(TEXT("SecondaryWeaponStorePoint"));
	SecondaryWeaponStorePoint->SetupAttachment(GetMesh(), FName(TEXT("spine_03")));
	
	SecondaryWeaponSheathRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondaryWeaponSheathRoot"));
	SecondaryWeaponSheathRoot->SetupAttachment(SecondaryWeaponStorePoint);
	
	SecondaryWeaponSheathTarget = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondaryWeaponSheathTarget"));
	SecondaryWeaponSheathTarget->SetupAttachment(SecondaryWeaponStorePoint);
	SecondaryWeaponSheathTarget->ComponentTags.Add(FName("SecondaryWeaponSheathTarget"));
	SecondaryWeaponSheathTarget->SetSimulatePhysics(true); // 这个设置对于武器摇曳非常重要
	
	SecondaryPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(
		TEXT("SecondaryPhysicsConstraint")
	);
	SecondaryPhysicsConstraint->SetupAttachment(SecondaryWeaponSheathRoot);
	SecondaryPhysicsConstraint->SetConstrainedComponents(
		SecondaryWeaponSheathRoot, FName(), SecondaryWeaponSheathTarget, FName()
	);
	SecondaryPhysicsConstraint->ConstraintInstance.SetLinearLimits(LCM_Limited, LCM_Limited, LCM_Limited, 1.25);
	SecondaryPhysicsConstraint->SetAngularSwing1Limit(ACM_Limited, 7.f);
	SecondaryPhysicsConstraint->SetAngularSwing2Limit(ACM_Limited, 10.f);
	SecondaryPhysicsConstraint->SetAngularTwistLimit(ACM_Limited, 10.f);
	SecondaryPhysicsConstraint->SetAngularDriveMode(EAngularDriveMode::TwistAndSwing);
	SecondaryPhysicsConstraint->SetAngularOrientationDrive(true, true);
	SecondaryPhysicsConstraint->SetAngularVelocityDriveTwistAndSwing(true, true);
}
