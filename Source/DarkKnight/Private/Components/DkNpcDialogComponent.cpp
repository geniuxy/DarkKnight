// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkNpcDialogComponent.h"

#include "DarkKnightDebugHelper.h"
#include "DkGameplayTags.h"
#include "Controllers/DkGamePlayerController.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "Subsytems/DkUISubsystem.h"
#include "Subsytems/EngineSubsystems/DkDataSubsystem.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "Widgets/Interact/DkWidgetDialogScreen.h"


UDkNpcDialogComponent::UDkNpcDialogComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	InteractMessage = TEXT("F - 对话");
}

bool UDkNpcDialogComponent::CanStartDialog()
{
	if (NpcId == 0 || StartDialogId == 0) return false;
	return true;
}

void UDkNpcDialogComponent::TryStartDialog(ADkGamePlayerController* PC)
{
	UDkUISubsystem* UISubsystem = UDkUISubsystem::Get(this);
	checkf(UISubsystem, TEXT("UISubsystem为空！"));

	UISubsystem->PushSoftWidgetToStackAsync(
		DkGameplayTags::Dk_WidgetStack_Interact,
		UDkUIFunctionLibrary::GetUISoftWidgetClassByTag(DkGameplayTags::Dk_Widget_DialogScreen),
		[=, this](EAsyncPushWidgetState InPushState, UDkWidgetActivatableBase* PushedWidget)
		{
			switch (InPushState)
			{
			case EAsyncPushWidgetState::OnCreatedBeforePush:
				{
					PushedWidget->SetOwningPlayer(PC);
					break;
				}
			case EAsyncPushWidgetState::AfterPush:
				CachedDialogScreen = CastChecked<UDkWidgetDialogScreen>(PushedWidget);
				CachedDialogScreen->BeginDialog(StartDialogId, this);
				break;
			}
		}
	);
}

void UDkNpcDialogComponent::CacheNpcTransform(TMap<int, FDialogNpcDetail> InNpcInfos)
{
	for (TTuple<int, FDialogNpcDetail> NpcInfo : InNpcInfos)
	{
		AActor* CurNpcActor = UDkDataSubsystem::Get()->GetNpcInfo().FindRef(NpcInfo.Key).NpcActor;
		if (!CurNpcActor)
		{
			Debug::Print(FString::Printf(TEXT("存初始位置时，没找到对应的Npc Actor, Id为: %d"), NpcInfo.Key));
			continue;
		}
		if (!CachedNpcTransforms.Contains(NpcInfo.Key))
		{
			CachedNpcTransforms.Add(NpcInfo.Key, CurNpcActor->GetActorTransform());
		}
	}
}

void UDkNpcDialogComponent::UpdateNpcTransform(TMap<int, FDialogNpcDetail> InNpcInfos)
{
	TMap<int, FNpcInfo> NpcInfoMap = UDkDataSubsystem::Get()->GetNpcInfo();
	AActor* MainNpcActor = NpcInfoMap.FindRef(NpcId).NpcActor;
	if (!MainNpcActor)
	{
		Debug::Print(FString::Printf(TEXT("没找到主Npc Actor, Id为: %d"), NpcId));
		return;
	}
	if (!InNpcInfos.Contains(NpcId))
	{
		Debug::Print(FString::Printf(TEXT("没找到对应的主Npc的信息, Id为: %d"), NpcId));
		return;
	}
	FDialogNpcDetail MainNpcDetail = InNpcInfos.FindRef(NpcId);
	// 如果主Npc的移动方式是瞬移，则先移动
	if (MainNpcDetail.MoveType == ENpcMoveType::Teleport)
	{
		MainNpcActor->SetActorTransform(MainNpcDetail.Transform);
	}
	
	for (TTuple<int, FDialogNpcDetail> NpcInfo : InNpcInfos)
	{
		AActor* CurNpcActor = UDkDataSubsystem::Get()->GetNpcInfo().FindRef(NpcInfo.Key).NpcActor;
		if (!CurNpcActor)
		{
			Debug::Print(FString::Printf(TEXT("没找到对应的Npc Actor, Id为: %d"), NpcInfo.Key));
			continue;
		}
		// 主Npc要么已经移动过了，要么不需要移动
		if (NpcInfo.Key == NpcId) continue;

		if (NpcInfo.Value.MoveType == ENpcMoveType::Teleport)
		{
			CurNpcActor->SetActorTransform(NpcInfo.Value.Transform);
		}
		else if (NpcInfo.Value.MoveType == ENpcMoveType::RelativeLocation)
		{
			// 如果MoveType=ENpcMoveType::RelativeLocation的话，就是把人物移动到主Npc前方RelativeDistance的位置，面对面
			FVector TargetLocation =
				MainNpcActor->GetActorForwardVector() * RelativeDistance + MainNpcActor->GetActorLocation();
			CurNpcActor->SetActorLocation(TargetLocation);

			FRotator MainNpcRotation = MainNpcActor->GetActorRotation();
			FRotator TargetRotation =
				FRotator(MainNpcRotation.Pitch, MainNpcRotation.Yaw + 180.f, MainNpcRotation.Roll);
			CurNpcActor->SetActorRotation(TargetRotation);
		}

		if (IsValid(NpcInfo.Value.TalkMontage))
		{
			if (ACharacter* CurNpcCharacter = Cast<ACharacter>(CurNpcActor))
			{
				CurNpcCharacter->PlayAnimMontage(NpcInfo.Value.TalkMontage);
			}
		}
	}
}

void UDkNpcDialogComponent::ResetNpcTransform()
{
	for (TTuple<int, FTransform> Pair : CachedNpcTransforms)
	{
		AActor* CurNpcActor = UDkDataSubsystem::Get()->GetNpcInfo().FindRef(Pair.Key).NpcActor;
		if (!CurNpcActor)
		{
			Debug::Print(FString::Printf(TEXT("重置位置时，没找到对应的Npc Actor, Id为: %d"), Pair.Key));
			continue;
		}

		CurNpcActor->SetActorTransform(Pair.Value);
	}
}

void UDkNpcDialogComponent::BeginPlay()
{
	Super::BeginPlay();

	UDkDataSubsystem::Get()->UpdateNpcInfo(NpcId, GetOwner());
}

