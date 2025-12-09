#pragma once

UENUM(BlueprintType)
enum class EDkTextJustify : uint8
{
	Left   UMETA(DisplayName = "Left"),
	Center UMETA(DisplayName = "Center"),
	Right  UMETA(DisplayName = "Right")
};