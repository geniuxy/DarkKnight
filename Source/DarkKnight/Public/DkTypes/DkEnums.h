#pragma once

UENUM(BlueprintType)
enum class EDkTextJustify : uint8
{
	Left UMETA(DisplayName = "Left"),
	Center UMETA(DisplayName = "Center"),
	Right UMETA(DisplayName = "Right")
};

UENUM(BlueprintType)
enum class EConfirmScreenType :uint8
{
	Ok,
	YesOrNo,
	OkOrCancel,
	Unknown UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EConfirmScreenButtonType : uint8
{
	Confirmed,
	Cancelled,
	Closed,
	Unknown UMETA(Hidden)
};
