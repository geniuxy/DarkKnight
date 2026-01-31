// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Composites/DkInventoryLeaf.h"

void UDkInventoryLeaf::ApplyFunction(FuncType Function)
{
	Function(this);
}
