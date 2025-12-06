// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/DkWidgetPrimaryLayout.h"

#include "DarkKnightDebugHelper.h"

UCommonActivatableWidgetContainerBase* UDkWidgetPrimaryLayout::FindWidgetStackByTag(const FGameplayTag& InTag) const
{
	checkf(RegisteredWidgetStackMap.Contains(InTag), TEXT("无法通过Tag(%s)找到Widget Stack"), *InTag.ToString());

	return RegisteredWidgetStackMap.FindRef(InTag); // FindRef只对外提供副本(“只读且安全”的查询)
}

void UDkWidgetPrimaryLayout::RegisterWidgetStack(FGameplayTag InStackTag,
                                                 UCommonActivatableWidgetContainerBase* InStack)
{
	if (!IsDesignTime())
	{
		if (!RegisteredWidgetStackMap.Contains(InStackTag))
		{
			RegisteredWidgetStackMap.Add(InStackTag, InStack);

			Debug::Print(FString::Printf(TEXT("Widget Stack 已通过 Tag(%s) 注册！"), *InStackTag.ToString()));
		}
	}
}
