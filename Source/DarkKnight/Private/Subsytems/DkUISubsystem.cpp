// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsytems/DkUISubsystem.h"

#include "DarkKnightDebugHelper.h"


UDkUISubsystem* UDkUISubsystem::Get(const UObject* WorldContextObject)
{
	if (GEngine)
	{
		UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);

		return UGameInstance::GetSubsystem<UDkUISubsystem>(World->GetGameInstance());
	}

	return nullptr;
}

bool UDkUISubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!CastChecked<UGameInstance>(Outer)->IsDedicatedServerInstance()) // 因为 UI 子系统通常不需要在服务器上运行
	{
		TArray<UClass*> FoundClasses;
		GetDerivedClasses(GetClass(), FoundClasses); // 获取当前类的所有派生类（子类）

		return FoundClasses.IsEmpty(); // 我不是专用服务器，而且没人继承我，我才允许自己被创建。
	}
	
	return false;
}

void UDkUISubsystem::RegisterCreatedPrimaryLayoutWidget(UDkWidgetPrimaryLayout* InCreatedWidget)
{
	check(InCreatedWidget);
	CreatedPrimaryLayout = InCreatedWidget;

	Debug::Print(TEXT("Primary Layout 已存于 UISubsystem 中！"));
}
