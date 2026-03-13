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
