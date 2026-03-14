// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Components/Buttons/DkUIButtonDescription.h"

#include "CommonTextBlock.h"
#include "Subsytems/DkUISubsystem.h"

void UDkUIButtonDescription::NativePreConstruct()
{
	Super::NativePreConstruct();

	CommonText_ButtonDescription->SetText(FText::GetEmpty());
}

void UDkUIButtonDescription::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	UDkUISubsystem::Get(this)->
		OnButtonDescriptionTextUpdated.AddDynamic(this, &ThisClass::HandleDescriptionTextUpdated);
}

void UDkUIButtonDescription::HandleDescriptionTextUpdated(UDkUICommonButtonBase* InButton, FText InDescriptionText)
{
	CommonText_ButtonDescription->SetText(InDescriptionText);
}
