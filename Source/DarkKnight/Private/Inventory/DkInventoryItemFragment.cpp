#include "Inventory/DkInventoryItemFragment.h"

#include "DarkKnightDebugHelper.h"
#include "Widgets/Inventory/Composites/DkInventoryLeaf.h"
#include "Widgets/Inventory/Composites/DkInventoryLeafImage.h"
#include "Widgets/Inventory/Composites/DkInventoryLeafText.h"

void FInventoryItemFragment::Assimilate(UDkInventoryCompositeBase* Composite) const
{
	if (!MatchesWidgetTag(Composite)) return;

	Composite->Expand();
}

bool FInventoryItemFragment::MatchesWidgetTag(const UDkInventoryCompositeBase* Composite) const
{
	return Composite->GetFragmentTag().MatchesTagExact(GetFragmentTag());
}

void FInventoryItemImageFragment::Assimilate(UDkInventoryCompositeBase* Composite) const
{
	FInventoryItemFragment::Assimilate(Composite);

	UDkInventoryLeafImage* Image = Cast<UDkInventoryLeafImage>(Composite);
	if (!IsValid(Image)) return;

	Image->SetImage(Icon);
	Image->SetBoxSize(IconDimension);
	Image->SetImageSize(IconDimension);
}

void FInventoryItemTextFragment::Assimilate(UDkInventoryCompositeBase* Composite) const
{
	FInventoryItemFragment::Assimilate(Composite);

	UDkInventoryLeafText* Text = Cast<UDkInventoryLeafText>(Composite);
	if (!IsValid(Text)) return;

	Text->SetText(FragmentText);
}

void FInventoryItemHealthConsumableFragment::OnConsume(APlayerController* PC)
{
	// Get a stats component from the PC or the PC->GetPawn()
	// or get the Ability System Component and apply a Gameplay Effect
	// or call an interface function for Healing()

	Debug::Print(FString::Printf(TEXT("血量相关Item已被使用！恢复量为: %f"), HealthAmount));
}

void FInventoryItemManaConsumableFragment::OnConsume(APlayerController* PC)
{
	// Replenish mana however you wish
	
	Debug::Print(FString::Printf(TEXT("法力值相关Item已被使用！恢复量为: %f"), ManaAmount));
}