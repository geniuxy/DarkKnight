// Fill out your copyright notice in the Description page of Project Settings.


#include "GAS/Abilities/Common/GA_SummonMount.h"

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
				if (OwnerAvatarCharacter->GetDistanceTo(OwnerPlayerState->GetCachedMount()) > CheckDistance * 2)
				{
					OwnerPlayerState->GetCachedMount()->Destroy();
					SpawnNewMount();
				}
				else
				{
					OwnerPlayerState->GetCachedMount()->SetActorLocation(FindPlaceToSummonMount());
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
	ADkMountBase* CachedMount = GetWorld()->SpawnActor<ADkMountBase>(
		MountClass, PlaceToSummonMount, FRotator::ZeroRotator, SpawnParams
	);
	CachedMount->SetInstigator(OwnerAvatarCharacter);
	OwnerPlayerState->SetCachedMount(CachedMount);
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

	// 2. 尝试右侧
	if (TryFindSummonLocationInDirection(OwnerAvatarCharacter->GetActorRightVector(), LocationToSummon))
	{
		return LocationToSummon;
	}

	// 3. 尝试左侧
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
		FVector CheckPoint = OwnerAvatarCharacter->GetActorLocation() - Direction * CheckDistance + i * CheckDistance / 8.f;

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

	// 3. 胶囊体检测（检查该位置是否有足够空间）
	FVector CheckCapsuleStartPoint = HitResult.Location + FVector(0.f, 0.f, 120.f);
	FVector CheckCapsuleEndPoint = CheckCapsuleStartPoint +
		FVector(DirectionFromHitResultToOwner.X * 150.f, DirectionFromHitResultToOwner.Y * 150.f, 0.f);

	FHitResult CapsuleHitResult;
	EDrawDebugTrace::Type DrawDebugTrace =
		bShouldDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	bool bCapsuleHit = UKismetSystemLibrary::CapsuleTraceSingle(
		this,
		CheckCapsuleStartPoint,
		CheckCapsuleEndPoint,
		45.f,
		90.f,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		TArray<AActor*>(),
		DrawDebugTrace,
		CapsuleHitResult,
		true
	);
	if (bCapsuleHit)
	{
		return false;
	}

	// 找到有效位置
	OutLocation = HitResult.Location + FVector(0.f, 0.f, 95.f);
	return true;
}
