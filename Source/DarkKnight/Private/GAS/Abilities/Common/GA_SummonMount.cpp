// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Common/GA_SummonMount.h"

#include "Animations/AnimInstances/DkAnimInstanceMountBase.h"
#include "Characters/DkCharacterBase.h"
#include "Characters/Mounts/DkMountBase.h"
#include "GameFramework/Character.h"
#include "Games/PlayerStates/DkPlayerStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UGA_SummonMount::UGA_SummonMount()
{
}

void UGA_SummonMount::ActivateAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!K2_CommitAbility())
	{
		K2_EndAbility();
		return;
	}

	if (!GetOwnerAvatarCharacter()) return;
	if (!GetOwnerPlayerState()) return;

	if (K2_HasAuthority())
	{
		if (!OwnerPlayerState->GetCachedMount())
		{
			SpawnNewMount();
		}
		else
		{
			if (GetOwnerAvatarCharacter())
			{
				if (OwnerAvatarCharacter->GetDistanceTo(OwnerPlayerState->GetCachedMount()) > CheckDistance)
				{
					OwnerPlayerState->GetCachedMount()->Destroy();
					SpawnNewMount();
				}
				else
				{
					FVector PlaceToSummonMount = FindPlaceToSummonMount();

					FCollisionShape Shape = FCollisionShape::MakeCapsule(45.f, 90.f);
					FCollisionQueryParams Params;
					Params.AddIgnoredActor(OwnerPlayerState->GetCachedMount());
					Params.AddIgnoredActor(OwnerAvatarCharacter);

					FHitResult SafetyCheck;
					bool bBlocked = GetWorld()->SweepSingleByChannel(
						SafetyCheck,
						PlaceToSummonMount,
						PlaceToSummonMount,
						FQuat::Identity,
						ECC_Pawn,
						Shape,
						Params
					);

					if (!bBlocked)
					{
						// TODO:命令马匹走向主角
						OwnerPlayerState->GetCachedMount()->SetActorLocation(PlaceToSummonMount);

						FVector DirectionToOwner = GetOwnerAvatarCharacter()->GetActorLocation() - PlaceToSummonMount;
						DirectionToOwner.Z = 0.f;
						DirectionToOwner.Normalize();
						OwnerPlayerState->GetCachedMount()->SetActorRotation(DirectionToOwner.Rotation());
					}
				}
			}
		}
	}

	UGameplayStatics::PlaySoundAtLocation(this, SummonSoundEffect, OwnerAvatarCharacter->GetActorLocation());

	K2_EndAbility();
}

void UGA_SummonMount::SpawnNewMount()
{
	if (!GetOwnerAvatarCharacter()) return;
	FVector PlaceToSummonMount = FindPlaceToSummonMount();
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FVector DirectionToOwner = GetOwnerAvatarCharacter()->GetActorLocation() - PlaceToSummonMount;
	DirectionToOwner.Z = 0.0f; // 保持水平朝向，忽略高度差（可选）
	DirectionToOwner.Normalize();
	FRotator SpawnRotation = DirectionToOwner.Rotation();

	ADkMountBase* CachedMount = GetWorld()->SpawnActor<ADkMountBase>(
		MountClass, PlaceToSummonMount, SpawnRotation, SpawnParams
	);
	UpdateInstigator(CachedMount);
	
	OwnerPlayerState->SetCachedMount(CachedMount);
}

void UGA_SummonMount::UpdateInstigator(ADkMountBase* CachedMount)
{
	if (CachedMount && OwnerAvatarCharacter)
	{
		CachedMount->SetInstigator(OwnerAvatarCharacter);
		ADkCharacterBase* OwnerCharacterBase = Cast<ADkCharacterBase>(OwnerAvatarCharacter);
		if (USkeletalMeshComponent* Mesh = CachedMount->GetMesh())
		{
			if (UDkAnimInstanceMountBase* AnimInst = Cast<UDkAnimInstanceMountBase>(Mesh->GetAnimInstance()))
			{
				AnimInst->SetOwnerInstigator(OwnerCharacterBase);
			}
		}
	}
}

FVector UGA_SummonMount::FindPlaceToSummonMount()
{
	if (!GetOwnerAvatarCharacter()) return FVector();

	FVector LocationToSummon = OwnerAvatarCharacter->GetActorLocation();

	// 1. 尝试后方（Forward的反方向）
	if (TryFindSummonLocationInDirection(OwnerAvatarCharacter->GetActorForwardVector(), LocationToSummon))
	{
		return LocationToSummon;
	}

	// 2. 尝试左侧
	if (TryFindSummonLocationInDirection(OwnerAvatarCharacter->GetActorRightVector(), LocationToSummon))
	{
		return LocationToSummon;
	}

	// 3. 尝试右侧
	if (TryFindSummonLocationInDirection(-OwnerAvatarCharacter->GetActorRightVector(), LocationToSummon))
	{
		return LocationToSummon;
	}

	return LocationToSummon;
}

bool UGA_SummonMount::TryFindSummonLocationInDirection(const FVector& Direction, FVector& OutLocation)
{
	for (int i = 0; i < 8; ++i)
	{
		FVector CheckPoint = OwnerAvatarCharacter->GetActorLocation() + Direction * (-CheckDistance + i * CheckDistance
			/ 8.f);

		if (CheckSummonLocationAtPoint(CheckPoint, OutLocation))
		{
			return true;
		}
	}
	return false;
}

bool UGA_SummonMount::CheckSummonLocationAtPoint(const FVector& CheckPoint, FVector& OutLocation)
{
	FVector StartPoint = CheckPoint + FVector(0.f, 0.f, 200.f);
	FVector EndPoint = CheckPoint + FVector(0.f, 0.f, -400.f);
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, StartPoint, EndPoint, ECC_Visibility);
	if (!HitResult.bBlockingHit)
	{
		return false;
	}

	// 2. 计算从命中点指向Owner的方向
	FVector DirectionFromHitResultToOwner = UKismetMathLibrary::GetDirectionUnitVector(
		HitResult.Location, OwnerAvatarCharacter->GetActorLocation()
	);

	// 胶囊体检测（检查该位置是否有足够空间）
	FVector GroundLocation = HitResult.Location + FVector(0.f, 0.f, 95.f);

	// 3. 胶囊体检测（检查该位置是否有足够空间）
	FVector CheckCapsuleStartPoint = GroundLocation -
		FVector(DirectionFromHitResultToOwner.X * 150.f, DirectionFromHitResultToOwner.Y * 150.f, 0.f);
	FVector CheckCapsuleEndPoint = GroundLocation +
		FVector(DirectionFromHitResultToOwner.X * 150.f, DirectionFromHitResultToOwner.Y * 150.f, 0.f);

	FHitResult CapsuleHitResult;
	EDrawDebugTrace::Type DrawDebugTrace =
		bShouldDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	TArray<AActor*> ActorsToIgnore;
	if (OwnerPlayerState->GetCachedMount())
	{
		ActorsToIgnore.Add(OwnerPlayerState->GetCachedMount());
	}
	bool bCapsuleHit = UKismetSystemLibrary::CapsuleTraceSingle(
		this,
		CheckCapsuleStartPoint,
		CheckCapsuleEndPoint,
		45.f,
		90.f,
		UEngineTypes::ConvertToTraceType(ECC_Pawn),
		false,
		ActorsToIgnore,
		DrawDebugTrace,
		CapsuleHitResult,
		false
	);
	if (bCapsuleHit)
	{
		return false;
	}

	// 找到有效位置
	OutLocation = GroundLocation;
	return true;
}
