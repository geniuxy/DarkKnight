// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/DkInventoryPlayerController.h"

#include "DarkKnightDebugHelper.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ADkInventoryPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (IsValid(InputSubsystem))
	{
		InputSubsystem->AddMappingContext(IMCInventory, 0);
	}
}

void ADkInventoryPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ThisClass::OnInteract);
}

void ADkInventoryPlayerController::OnInteract()
{
	Debug::Print(TEXT("我正在交互！"));
}
