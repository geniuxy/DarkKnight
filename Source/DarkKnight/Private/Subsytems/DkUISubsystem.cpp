// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsytems/DkUISubsystem.h"

#include "DarkKnightDebugHelper.h"
#include "DkGameplayTags.h"
#include "FunctionLibrarys/DkUIFunctionLibrary.h"
#include "Engine/AssetManager.h"
#include "Widgets/CommonActivatableWidgetContainer.h"
#include "Widgets/DkWidgetPrimaryLayout.h"
#include "Widgets/DkWidgetActivatableBase.h"
#include "Widgets/DkWidgetConfirmScreen.h"


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
}

void UDkUISubsystem::PushSoftWidgetToStackAsync(
	const FGameplayTag& InWidgetStackTag,
	TSoftClassPtr<UDkWidgetActivatableBase> InSoftWidgetClass,
	TFunction<void(EAsyncPushWidgetState, UDkWidgetActivatableBase*)> AsyncPushStateCallback)
{
	checkf(!InSoftWidgetClass.IsNull(), TEXT("InSoftWidgetClass不能为空"));

	UAssetManager::Get().GetStreamableManager().RequestAsyncLoad(
		InSoftWidgetClass.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this, InSoftWidgetClass, InWidgetStackTag, AsyncPushStateCallback]()
			{
				UClass* LoadedWidgetClass = InSoftWidgetClass.Get();

				checkf(LoadedWidgetClass && CreatedPrimaryLayout, TEXT("LoadedWidgetClass或者CreatedPrimaryLayout为空"));

				UCommonActivatableWidgetContainerBase* FoundWidgetStack =
					CreatedPrimaryLayout->FindWidgetStackByTag(InWidgetStackTag);
				UDkWidgetActivatableBase* CreatedWidget = FoundWidgetStack->AddWidget<UDkWidgetActivatableBase>(
					LoadedWidgetClass,
					[AsyncPushStateCallback](UDkWidgetActivatableBase& CreatedWidgetInstance)
					{
						AsyncPushStateCallback(EAsyncPushWidgetState::OnCreatedBeforePush, &CreatedWidgetInstance);
					}
				);

				AsyncPushStateCallback(EAsyncPushWidgetState::AfterPush, CreatedWidget);
			}
		)
	);
}

void UDkUISubsystem::PushConfirmScreenToModalStackAsync(
	EConfirmScreenType InScreenType,
	const FText& InScreenText,
	const FText& InScreenMsg,
	TFunction<void(EConfirmScreenButtonType)> ButtonClickedCallback)
{
	UConfirmScreenInfoObject* ConfirmScreenInfoObject = nullptr;

	switch (InScreenType)
	{
	case EConfirmScreenType::Ok:
		ConfirmScreenInfoObject = UConfirmScreenInfoObject::CreateOKScreen(InScreenText, InScreenMsg);
		break;
	case EConfirmScreenType::YesOrNo:
		ConfirmScreenInfoObject = UConfirmScreenInfoObject::CreateYesOrNoScreen(InScreenText, InScreenMsg);
		break;
	case EConfirmScreenType::OkOrCancel:
		ConfirmScreenInfoObject = UConfirmScreenInfoObject::CreateOKOrCancelScreen(InScreenText, InScreenMsg);
		break;
	default:
		break;
	}

	check(ConfirmScreenInfoObject);

	PushSoftWidgetToStackAsync(
		DkGameplayTags::Dk_WidgetStack_Modal,
		UDkUIFunctionLibrary::GetUISoftWidgetClassByTag(DkGameplayTags::Dk_Widget_ConfirmScreen),
		[ConfirmScreenInfoObject, ButtonClickedCallback](
		EAsyncPushWidgetState InPushState, UDkWidgetActivatableBase* PushedWidget)
		{
			if (InPushState == EAsyncPushWidgetState::OnCreatedBeforePush)
			{
				UDkWidgetConfirmScreen* ConfirmScreen = CastChecked<UDkWidgetConfirmScreen>(PushedWidget);
				ConfirmScreen->InitConfirmScreen(ConfirmScreenInfoObject, ButtonClickedCallback);
			}
		}
	);
}
