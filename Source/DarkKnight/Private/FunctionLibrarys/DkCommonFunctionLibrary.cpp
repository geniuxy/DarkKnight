// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrarys/DkCommonFunctionLibrary.h"

bool UDkCommonFunctionLibrary::IsRightMouseClick(const FPointerEvent& MouseEvent)
{
	return MouseEvent.GetEffectingButton() == EKeys::RightMouseButton;
}

bool UDkCommonFunctionLibrary::IsLeftMouseClick(const FPointerEvent& MouseEvent)
{
	return MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
}