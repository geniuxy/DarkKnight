// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "EngineUtils.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DkGameFunctionLibrary.generated.h"

class ADkCharacterBase;
/**
 * 
 */
UCLASS()
class DARKKNIGHT_API UDkGameFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 获取范围内所有对应类的单位
	template <typename T>
	static TArray<T*> GetUnitsInRange(
		const UObject* WorldContextObject, FVector Center, float Range, int32 TeamId = -1
	);

	// 带过滤的 获取范围内所有对应类的单位，使用示例如下：
	// UDkGameFunctionLibrary::GetUnitsInRange<ADkCharacterBase>(
	// 	this, GetActorLocation(), 1000.0f,
	// [](ADkCharacterBase* Unit) -> bool
	// {
	// 	return Unit->IsAlive() && Unit->GetHealthPercent() >= 1.0f;
	// });
	template <typename T>
	static TArray<T*> GetUnitsInRange(
		const UObject* WorldContextObject, FVector Center, float Range, TFunctionRef<bool(T*)> Predicate
	);
};

template <typename T>
TArray<T*> UDkGameFunctionLibrary::GetUnitsInRange(
	const UObject* WorldContextObject, FVector Center, float Range, int32 TeamId)
{
	TArray<T*> Result;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World || Range <= 0.0f) return Result;

	const float RangeSq = FMath::Square(Range);

	for (TActorIterator<T> It(World); It; ++It)
	{
		T* Unit = *It;
		if (!IsValid(Unit)) continue;
		// 距离检测
		if (FVector::DistSquared(Center, Unit->GetActorLocation()) > RangeSq) continue;
		// TeamId 检测
		if (IGenericTeamAgentInterface* TeamInterface = Cast<IGenericTeamAgentInterface>(Unit))
		{
			if (TeamId != -1 && TeamInterface->GetGenericTeamId() != TeamId) continue;

			Result.Add(Unit);
		}
	}

	return Result;
}

template <typename T>
TArray<T*> UDkGameFunctionLibrary::GetUnitsInRange(
	const UObject* WorldContextObject, FVector Center, float Range, TFunctionRef<bool(T*)> Predicate)
{
	TArray<T*> Result;
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if (!World || Range <= 0.0f) return Result;

	const float RangeSq = FMath::Square(Range);

	for (TActorIterator<T> It(World); It; ++It)
	{
		T* Unit = *It;
		if (!IsValid(Unit)) continue;

		if (FVector::DistSquared(Center, Unit->GetActorLocation()) <= RangeSq && Predicate(Unit))
		{
			Result.Add(Unit);
		}
	}

	return Result;
}
