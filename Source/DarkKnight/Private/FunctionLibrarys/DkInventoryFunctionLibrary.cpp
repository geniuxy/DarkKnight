// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionLibrarys/DkInventoryFunctionLibrary.h"

#include "DkGameplayTags.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "Characters/DkCharacterHero.h"
#include "Components/DkInventoryComponent.h"
#include "Components/Widget.h"

int32 UDkInventoryFunctionLibrary::GetIndexFromPosition(const FIntPoint& Position, const int32 Columns)
{
	return Position.X + Position.Y * Columns;
}

FIntPoint UDkInventoryFunctionLibrary::GetPositionFormIndex(const int32 Index, const int32 Columns)
{
	return FIntPoint(Index % Columns, Index / Columns);
}

UDkInventoryComponent* UDkInventoryFunctionLibrary::GetInventoryComponent(const APlayerController* PlayerController)
{
	if (!IsValid(PlayerController)) return nullptr;
	UDkInventoryComponent* InventoryComponent =
		CastChecked<ADkCharacterHero>(PlayerController->GetPawn())->FindComponentByClass<UDkInventoryComponent>();
	return InventoryComponent;
}

FVector2D UDkInventoryFunctionLibrary::GetWidgetPosition(UWidget* Widget)
{
	const FGeometry Geometry = Widget->GetCachedGeometry();
	FVector2D PixelPosition; // 物理像素, 操作系统认的硬件像素
	FVector2D ViewportPosition; // 逻辑/虚拟像素, 数值不受 DPI 缩放影响
	USlateBlueprintLibrary::LocalToViewport(
		Widget, Geometry, USlateBlueprintLibrary::GetLocalTopLeft(Geometry), PixelPosition, ViewportPosition
	);
	return ViewportPosition;
}

FVector2D UDkInventoryFunctionLibrary::GetWidgetSize(UWidget* Widget)
{
	const FGeometry Geometry = Widget->GetCachedGeometry();
	return Geometry.GetLocalSize();
}

bool UDkInventoryFunctionLibrary::IsWithInBounds(
	const FVector2D& BoundaryPos, const FVector2D& WidgetSize, const FVector2D& MousePos)
{
	return MousePos.X >= BoundaryPos.X && MousePos.X <= (BoundaryPos.X + WidgetSize.X) &&
		MousePos.Y >= BoundaryPos.Y && MousePos.Y <= (BoundaryPos.Y + WidgetSize.Y);
}

FGameplayTag UDkInventoryFunctionLibrary::GetMainEntryTagByIndex(int32 InIndex)
{
	checkf(InIndex >= 0 && InIndex < MAX_MAIN_ENTRY_NUM, TEXT("主词条的数目不符合规定！"));
	switch (InIndex)
	{
	case 0:
		return DkGameplayTags::Dk_Inventory_Fragment_Entry_Main_0;
	case 1:
		return DkGameplayTags::Dk_Inventory_Fragment_Entry_Main_1;
	default:
		break;
	}
	return FGameplayTag::EmptyTag;
}

FGameplayTag UDkInventoryFunctionLibrary::GetSubEntryTagByIndex(int32 InIndex)
{
	checkf(InIndex >= 0 && InIndex < MAX_SUB_ENTRY_NUM, TEXT("子词条的数目不符合规定！"));
	switch (InIndex)
	{
	case 0:
		return DkGameplayTags::Dk_Inventory_Fragment_Entry_Sub_0;
	case 1:
		return DkGameplayTags::Dk_Inventory_Fragment_Entry_Sub_1;
	case 2:
		return DkGameplayTags::Dk_Inventory_Fragment_Entry_Sub_2;
	case 3:
		return DkGameplayTags::Dk_Inventory_Fragment_Entry_Sub_3;
	default:
		break;
	}
	return FGameplayTag::EmptyTag;
}
