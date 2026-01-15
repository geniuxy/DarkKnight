// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsytems/DkInventorySubsystem.h"

UDkInventorySubsystem* UDkInventorySubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

		return UGameInstance::GetSubsystem<UDkInventorySubsystem>(World->GetGameInstance());
	}

	return nullptr;
}

bool UDkInventorySubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	// 目前， InventorySubsystem先不需要在服务器上运行
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance())
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses); // 获取当前类的所有派生类（子类）

		return FoundClasses.IsEmpty(); // 我不是专用服务器，而且没人继承我，我才允许自己被创建。
	}

	return false;
}

void UDkInventorySubsystem::RegisterCachedInventoryComponent(UDkInventoryComponent* InventoryComponent)
{
	check(InventoryComponent);
	CachedInventoryComponent = InventoryComponent;
}
