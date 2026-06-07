// Fill out your copyright notice in the Description page of Project Settings.


#include "Inputs/InputTriggers/InputTrigger_ToggleOrHoldSprint.h"

#include "Settings/DkGameUserSettings.h"

ETriggerState UInputTrigger_ToggleOrHoldSprint::UpdateState_Implementation(
	const UEnhancedPlayerInput* PlayerInput, FInputActionValue ModifiedValue, float DeltaTime)
{
	// 必须在 Super 之前保存上一帧状态，不然基类调用后会更新
	const bool bLastActuated = IsActuated(LastValue);
	ETriggerState CurTriggerState = Super::UpdateState_Implementation(PlayerInput, ModifiedValue, DeltaTime);
	const bool bIsActuated = IsActuated(ModifiedValue);

	bool bToggle;
	if (IsValid(UDkGameUserSettings::Get()))
	{
		bToggle = UDkGameUserSettings::Get()->IsToggleSprintMode();
	}
	else
	{
		bToggle = bIsToggle;
	}
	
	if (bToggle) // Toggle Logic
	{
		// 具体参考UInputTriggerPressed
		if (!bLastActuated && bIsActuated)
		{
			CurTriggerState = ETriggerState::Triggered;
		}
	}
	else // Hold Logic
	{
		// 具体参考UInputTriggerPressed和UInputTriggerRelease
		if (bIsActuated && !bLastActuated)
		{
			// 刚按下：触发一次（激活 Ability）
			CurTriggerState = ETriggerState::Triggered;
		}
		else if (!bIsActuated && bLastActuated)
		{
			// 松开：触发一次（触发 WaitInputRelease → EndAbility）
			CurTriggerState = ETriggerState::Triggered;
		}
		else if (bIsActuated)
		{
			// 持续按住：返回 Ongoing（保持 Ability 激活）
			CurTriggerState = ETriggerState::Ongoing;
		}
		else
		{
			CurTriggerState = ETriggerState::None;
		}
	}
	
	return CurTriggerState;
}
