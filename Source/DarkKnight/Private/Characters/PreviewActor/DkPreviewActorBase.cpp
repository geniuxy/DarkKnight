// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PreviewActor/DkPreviewActorBase.h"

#include "Components/DkEquipmentComponent.h"
#include "DarkKnight/DarkKnight.h"
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
	if (SourceMesh->GetAnimInstance())
	{
		Mesh->SetAnimInstanceClass(SourceMesh->GetAnimInstance()->GetClass());
	}

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
	PrimaryWeaponStorePoint->SetRelativeLocation(FVector(26.799625f, 1.694577f, -0.34177f));
	
	PrimaryWeaponSheathRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrimaryWeaponSheathRoot"));
	PrimaryWeaponSheathRoot->SetupAttachment(PrimaryWeaponStorePoint);
	PrimaryWeaponSheathRoot->SetRelativeLocation(FVector(5.117355f, -20.400218f, 0.732806f));
	PrimaryWeaponSheathRoot->SetRelativeScale3D(FVector(0.05f));
	PrimaryWeaponSheathRoot->SetHiddenInGame(true);
	PrimaryWeaponSheathRoot->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	PrimaryWeaponSheathRoot->SetCollisionObjectType(ECC_WorldStatic);
	PrimaryWeaponSheathRoot->SetCollisionResponseToAllChannels(ECR_Ignore);
	PrimaryWeaponSheathRoot->SetCollisionResponseToChannel(ECC_Horse, ECR_Block);
	PrimaryWeaponSheathRoot->SetCollisionResponseToChannel(ECC_Fire, ECR_Block);
	
	PrimaryWeaponSheathTarget = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PrimaryWeaponSheathTarget"));
	PrimaryWeaponSheathTarget->SetupAttachment(PrimaryWeaponStorePoint);
	PrimaryWeaponSheathTarget->SetRelativeLocation(FVector(24.814073f, -15.426015f, 11.679205f));
	PrimaryWeaponSheathTarget->SetRelativeRotation(FRotator(-1.771982f, 98.242094f, 22.596763f));
	PrimaryWeaponSheathTarget->SetRelativeScale3D(FVector(0.05f));
	PrimaryWeaponSheathTarget->SetSimulatePhysics(true);
	PrimaryWeaponSheathTarget->ComponentTags.Add(FName("PrimaryWeaponSheathTarget"));
	PrimaryWeaponSheathTarget->SetHiddenInGame(true);
	PrimaryWeaponSheathTarget->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	PrimaryWeaponSheathTarget->SetCollisionObjectType(ECC_WorldStatic);
	PrimaryWeaponSheathTarget->SetCollisionResponseToAllChannels(ECR_Ignore);
	PrimaryWeaponSheathTarget->SetCollisionResponseToChannel(ECC_WeaponTrace, ECR_Block);
	PrimaryWeaponSheathTarget->SetCollisionResponseToChannel(ECC_Horse, ECR_Block);
	PrimaryWeaponSheathTarget->SetCollisionResponseToChannel(ECC_Fire, ECR_Block);
	
	PrimaryPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PrimaryPhysicsConstraint"));
	PrimaryPhysicsConstraint->SetupAttachment(PrimaryWeaponSheathRoot);
	PrimaryPhysicsConstraint->SetRelativeLocation(FVector(42.43798f, 28.933665f, 41.575463f));
	PrimaryPhysicsConstraint->SetRelativeRotation(FRotator(0.325678f, 91.504149f, 5.713538f));
	PrimaryPhysicsConstraint->SetRelativeScale3D(FVector(20.f));
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
	SecondaryWeaponStorePoint->SetRelativeLocation(FVector(26.238695f, -1.735423f, 4.68238f));
	
	SecondaryWeaponSheathRoot = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondaryWeaponSheathRoot"));
	SecondaryWeaponSheathRoot->SetupAttachment(SecondaryWeaponStorePoint);
	SecondaryWeaponSheathRoot->SetRelativeLocation(FVector(-3.000946f, -15.559848f, 4.164032f));
	SecondaryWeaponSheathRoot->SetRelativeScale3D(FVector(0.05f));
	SecondaryWeaponSheathRoot->SetHiddenInGame(true);
	SecondaryWeaponSheathRoot->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SecondaryWeaponSheathRoot->SetCollisionObjectType(ECC_WorldStatic);
	SecondaryWeaponSheathRoot->SetCollisionResponseToAllChannels(ECR_Overlap);
	SecondaryWeaponSheathRoot->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	SecondaryWeaponSheathRoot->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	SecondaryWeaponSheathRoot->SetCollisionResponseToChannel(ECC_Item, ECR_Ignore);
	SecondaryWeaponSheathRoot->SetCollisionResponseToChannel(ECC_WeaponTrace, ECR_Ignore);
	SecondaryWeaponSheathRoot->SetCollisionResponseToChannel(ECC_Horse, ECR_Block);
	SecondaryWeaponSheathRoot->SetCollisionResponseToChannel(ECC_Fire, ECR_Block);
	
	SecondaryWeaponSheathTarget = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SecondaryWeaponSheathTarget"));
	SecondaryWeaponSheathTarget->SetupAttachment(SecondaryWeaponStorePoint);
	SecondaryWeaponSheathTarget->SetRelativeLocation(FVector(15.954935f, -16.452876f, 16.861984f));
	SecondaryWeaponSheathTarget->SetRelativeRotation(FRotator(-8.280974f, 89.02337f, 34.799668f));
	SecondaryWeaponSheathTarget->SetRelativeScale3D(FVector(0.05f));
	SecondaryWeaponSheathTarget->SetSimulatePhysics(true);
	SecondaryWeaponSheathTarget->ComponentTags.Add(FName("SecondaryWeaponSheathTarget"));
	SecondaryWeaponSheathTarget->SetHiddenInGame(true);
	SecondaryWeaponSheathTarget->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SecondaryWeaponSheathTarget->SetCollisionObjectType(ECC_WorldStatic);
	SecondaryWeaponSheathTarget->SetCollisionResponseToAllChannels(ECR_Overlap);
	SecondaryWeaponSheathTarget->SetCollisionResponseToChannel(ECC_Visibility, ECR_Ignore);
	SecondaryWeaponSheathTarget->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	SecondaryWeaponSheathTarget->SetCollisionResponseToChannel(ECC_Item, ECR_Ignore);
	SecondaryWeaponSheathTarget->SetCollisionResponseToChannel(ECC_WeaponTrace, ECR_Block);
	SecondaryWeaponSheathTarget->SetCollisionResponseToChannel(ECC_Horse, ECR_Block);
	SecondaryWeaponSheathTarget->SetCollisionResponseToChannel(ECC_Fire, ECR_Block);
	
	SecondaryPhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(
		TEXT("SecondaryPhysicsConstraint")
	);
	SecondaryPhysicsConstraint->SetupAttachment(SecondaryWeaponSheathRoot);
	SecondaryPhysicsConstraint->SetRelativeLocation(FVector(41.150897f, 18.857516f, 45.847589f));
	SecondaryPhysicsConstraint->SetRelativeRotation(FRotator(0.325678f, 91.504149f, 5.713538f));
	SecondaryPhysicsConstraint->SetRelativeScale3D(FVector(20.f));
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
