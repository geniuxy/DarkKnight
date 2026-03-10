// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrarys/DkCommonFunctionLibrary.h"

bool UDkCommonFunctionLibrary::IsStringPureNumber(const FString& Str)
{
	if (Str.IsEmpty())
	{
		return false;
	}

	bool bHasDecimalPoint = false;
	bool bHasDigit = false;
    
	for (int32 i = 0; i < Str.Len(); ++i)
	{
		const TCHAR Char = Str[i];
        
		// 负号只能出现在第一位
		if (Char == TEXT('-'))
		{
			if (i != 0)
			{
				return false;  // 负号不在开头，非法
			}
			continue;
		}
        
		// 小数点只能出现一次
		if (Char == TEXT('.'))
		{
			if (bHasDecimalPoint)
			{
				return false;  // 多个小数点，非法
			}
			bHasDecimalPoint = true;
			continue;
		}
        
		// 必须是数字
		if (!FChar::IsDigit(Char))
		{
			return false;
		}
        
		bHasDigit = true;
	}
    
	// 至少有一个数字（防止 "." 或 "-" 这种情况）
	return bHasDigit;
}

bool UDkCommonFunctionLibrary::IsRightMouseClick(const FPointerEvent& MouseEvent)
{
	return MouseEvent.GetEffectingButton() == EKeys::RightMouseButton;
}

bool UDkCommonFunctionLibrary::IsLeftMouseClick(const FPointerEvent& MouseEvent)
{
	return MouseEvent.GetEffectingButton() == EKeys::LeftMouseButton;
}