// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Inventory/Composites/DkInventoryComposite.h"

#include "Blueprint/WidgetTree.h"

void UDkInventoryComposite::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WidgetTree->ForEachWidget(
		[this](UWidget* Widget)
		{
			if (UDkInventoryCompositeBase* Composite = Cast<UDkInventoryCompositeBase>(Widget); IsValid(Composite))
			{
				Children.Add(Composite);
				Composite->Collapse();
			}
		}
	);
}

void UDkInventoryComposite::ApplyFunction(FuncType Function)
{
	for (auto& Child: Children)
	{
		Child->ApplyFunction(Function);
	}
}

void UDkInventoryComposite::Collapse()
{
	for (auto& Child: Children)
	{
		Child->Collapse();
	}
}
