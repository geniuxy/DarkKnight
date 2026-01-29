// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/DkInventoryItemDescriptionMenu.h"

#include "Components/SizeBox.h"

FVector2D UDkInventoryItemDescriptionMenu::GetBoxSize() const
{
	return SizeBox_Root->GetDesiredSize();
}
