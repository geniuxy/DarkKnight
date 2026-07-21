// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkPlayerDialogComponent.h"

#include "DarkKnightDebugHelper.h"
#include "DkGameplayTags.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Characters/DkCharacterHero.h"
#include "Components/CapsuleComponent.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Subsytems/DkUISubsystem.h"
#include "Subsytems/EngineSubsystems/DkDataSubsystem.h"
#include "Widgets/GameMenu/ShopMenu/DkWidgetShopMenuScreen.h"

UDkPlayerDialogComponent::UDkPlayerDialogComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CachedDialogTags.Reset();
}

bool UDkPlayerDialogComponent::FindDialogGameplayTag(FGameplayTagContainer InTags)
{
	return CachedDialogTags.HasAllExact(InTags);
}

void UDkPlayerDialogComponent::AddDialogTag(FGameplayTag InTag)
{
	CachedDialogTags.AddTag(InTag);
}

void UDkPlayerDialogComponent::UpdateCameraFocus(
	int CameraFocusNpcId, EDialogCameraType CameraType, const FTransform& CustomCameraTransform)
{
	TMap<int, FNpcInfo> NpcInfoMap = UDkDataSubsystem::Get()->GetNpcInfo();
	if (NpcInfoMap.IsEmpty()) return;

	if (!NpcInfoMap.Contains(CameraFocusNpcId)) // CameraFocusNpcId为0的时候，用来还原摄像头
	{
		if (CustomCameraTransform.GetLocation() == FVector(0.f))
		{
			if (OwnerPC && CachedOwner)
			{
				OwnerPC->SetViewTargetWithBlend(CachedOwner);
			}
			if (CachedCameraActor)
			{
				FDetachmentTransformRules Rules(EDetachmentRule::KeepRelative, false);
				CachedCameraActor->DetachFromActor(Rules);
			}
		}
		else
		{
			UseCustomCameraTransform(CustomCameraTransform);
		}
	}
	else
	{
		AActor* CameraFocusNpcActor = NpcInfoMap.FindRef(CameraFocusNpcId).NpcActor;
		if (!CameraFocusNpcActor) return;
		if (!DialogCameraTransformMap.Contains(CameraType))
		{
			UseCustomCameraTransform(CustomCameraTransform);
			return;
		}

		if (CachedCameraActor)
		{
			FAttachmentTransformRules Rules(EAttachmentRule::KeepRelative, false);
			CachedCameraActor->AttachToActor(CameraFocusNpcActor, Rules);

			FVector TargetLocation = DialogCameraTransformMap.FindRef(CameraType).GetLocation();
			if (CameraFocusNpcActor->FindComponentByClass(UCapsuleComponent::StaticClass()))
			{
				float CameraHeight =
					CameraFocusNpcActor->GetComponentByClass<UCapsuleComponent>()->GetScaledCapsuleHalfHeight() / 1.5f;
				TargetLocation = FVector(TargetLocation.X, TargetLocation.Y, CameraHeight);
			}
			CachedCameraActor->SetActorRelativeLocation(TargetLocation);

			FRotator TargetRotator = DialogCameraTransformMap.FindRef(CameraType).GetRotation().Rotator();
			CachedCameraActor->SetActorRelativeRotation(TargetRotator);

			// 这里配置Transform的Scale的X用来传递FOV
			double TargetFOV = DialogCameraTransformMap.FindRef(CameraType).GetScale3D().X;
			CachedCameraActor->GetCameraComponent()->SetFieldOfView(TargetFOV);

			if (OwnerPC)
			{
				OwnerPC->SetViewTargetWithBlend(CachedCameraActor);
			}
		}
	}
}

void UDkPlayerDialogComponent::UseCustomCameraTransform(const FTransform& CustomCameraTransform)
{
	if (CachedCameraActor)
	{
		CachedCameraActor->SetActorTransform(CustomCameraTransform);

		double TargetFOV = CustomCameraTransform.GetScale3D().X;
		CachedCameraActor->GetCameraComponent()->SetFieldOfView(TargetFOV);

		if (OwnerPC)
		{
			OwnerPC->SetViewTargetWithBlend(CachedCameraActor);
		}
	}
}

void UDkPlayerDialogComponent::BeginPlay()
{
	Super::BeginPlay();

	CachedOwner = Cast<ADkCharacterHero>(GetOwner());
	if (CachedOwner)
	{
		OwnerPC = Cast<APlayerController>(CachedOwner->GetController());
	}

	// 预生成对话所需的Camera
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	CachedCameraActor = GetWorld()->SpawnActor<ACameraActor>(SpawnParams);
	CachedCameraActor->GetCameraComponent()->SetConstraintAspectRatio(false); // 去除电影质感的黑边框

	// 更新对话相关信息，Id=1为玩家自己
	if (GetOwner() == UGameplayStatics::GetPlayerPawn(this, 0))
	{
		UDkDataSubsystem::Get()->UpdateNpcInfo(1, GetOwner());
	}

	OnDialogBranchEventTriggered.AddUObject(this, &ThisClass::HandleDialogBranchEventTriggered);
}

void UDkPlayerDialogComponent::HandleDialogBranchEventTriggered(int InNpcId, const FDialogBranchInfo& InDialogBranchInfo)
{
	Debug::Print(TEXT("触发的事件为"), InDialogBranchInfo.TriggerEvent.ToString());

	if (InDialogBranchInfo.TriggerEvent == DkGameplayTags::Dk_Dialog_Branch_TriggerEvent_OpenShop)
	{
		HandleOpenShop(InNpcId, InDialogBranchInfo);
	}
}

void UDkPlayerDialogComponent::HandleOpenShop(int InNpcId, const FDialogBranchInfo& InDialogBranchInfo)
{
	UDkUISubsystem::Get(this)->PushSoftWidgetToStackAsync(
		DkGameplayTags::Dk_WidgetStack_GameMenu,
		UDkUIFunctionLibrary::GetUISoftWidgetClassByTag(DkGameplayTags::Dk_Widget_ShopMenu),
		[=, this](EAsyncPushWidgetState InPushState, UDkWidgetActivatableBase* PushedWidget)
		{
			if (InPushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UDkUIFunctionLibrary::ToggleInputMode(this, EDkInputMode::UIOnly, false);
			}
			if (InPushState == EAsyncPushWidgetState::AfterPush)
			{
				UDkWidgetShopMenuScreen* GameMenuScreen = CastChecked<UDkWidgetShopMenuScreen>(PushedWidget);
				GameMenuScreen->ConfigureShopMenu(InNpcId, InDialogBranchInfo.JumpToContentId);
			}
		}
	);
}
