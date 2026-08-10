// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/Buttons/Craft/DkUICraftReferenceButton.h"

void UDkUICraftReferenceButton::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	IUserObjectListEntry::NativeOnListItemObjectSet(ListItemObject);
	
}

void UDkUICraftReferenceButton::NativeOnClicked()
{
	Super::NativeOnClicked();
}
