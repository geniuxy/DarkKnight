// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/GameMenu/DkWidgetGameMenuScreen.h"

void UDkWidgetGameMenuScreen::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();

	FInputModeGameOnly InputModeData;
	GetOwningPlayerController()->SetInputMode(InputModeData);
	GetOwningPlayerController()->bShowMouseCursor = false;
}
