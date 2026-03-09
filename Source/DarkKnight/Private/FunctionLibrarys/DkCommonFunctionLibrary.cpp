// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrarys/DkCommonFunctionLibrary.h"

bool UDkCommonFunctionLibrary::IsStringPureNumber(const FString& Str)
{
	if (Str.IsEmpty())
	{
		return false;
	}
    
	for (const TCHAR& Char : Str)
	{
		if (!FChar::IsDigit(Char))
		{
			return false;
		}
	}
	return true;

}

bool UDkCommonFunctionLibrary::IsRightMouseClick(const FPointerEvent& MouseEvent)
{
	return MouseEvent.GetEffectingButton() == EKeys::RightMouseButton;
}

bool UDkCommonFunctionLibrary::IsLeftMouseClick(const FPointerEvent& MouseEvent)
{
	return MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
}