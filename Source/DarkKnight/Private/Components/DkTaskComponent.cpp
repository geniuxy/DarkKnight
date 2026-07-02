// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/DkTaskComponent.h"

#include "DarkKnightDebugHelper.h"


UDkTaskComponent::UDkTaskComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDkTaskComponent::UpdatePlayerTaskCompletionStatus()
{
	Debug::Print("初始化当前任务进度");
}

void UDkTaskComponent::BeginPlay()
{
	Super::BeginPlay();
}

