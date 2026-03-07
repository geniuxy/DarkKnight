// Fill out your copyright notice in the Description page of Project Settings.


#include "DkGameplayTags.h"

namespace DkGameplayTags
{
	// Input Actions
	UE_DEFINE_GAMEPLAY_TAG(Dk_Input_Action_Move, "Dk.Input.Action.Move");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Input_Action_ToggleMoveStyle, "Dk.Input.Action.ToggleMoveStyle");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Input_Action_Look, "Dk.Input.Action.Look");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Input_Action_Jump, "Dk.Input.Action.Jump");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Input_Action_Interact, "Dk.Input.Action.Interact");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Input_Action_OpenInventory, "Dk.Input.Action.OpenInventory");
	
	// UI Widget Stack
	UE_DEFINE_GAMEPLAY_TAG(Dk_WidgetStack_Modal, "Dk.WidgetStack.Modal");
	UE_DEFINE_GAMEPLAY_TAG(Dk_WidgetStack_GameMenu, "Dk.WidgetStack.GameMenu");
	UE_DEFINE_GAMEPLAY_TAG(Dk_WidgetStack_Interact, "Dk.WidgetStack.Interact");
	UE_DEFINE_GAMEPLAY_TAG(Dk_WidgetStack_GameHud, "Dk.WidgetStack.GameHud");
	UE_DEFINE_GAMEPLAY_TAG(Dk_WidgetStack_Frontend, "Dk.WidgetStack.Frontend");

	// UI Widgets
	UE_DEFINE_GAMEPLAY_TAG(Dk_Widget_PressAnyKeyScreen, "Dk.Widget.PressAnyKeyScreen");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Widget_MainMenuScreen, "Dk.Widget.MainMenuScreen");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Widget_StartGameScreen, "Dk.Widget.StartGameScreen");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Widget_OptionScreen, "Dk.Widget.OptionScreen");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Widget_ConfirmScreen, "Dk.Widget.ConfirmScreen");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Widget_KeyRemapScreen, "Dk.Widget.KeyRemapScreen");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Widget_CreditScreen, "Dk.Widget.CreditScreen");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Widget_GameHUD, "Dk.Widget.GameHUD");

	UE_DEFINE_GAMEPLAY_TAG(Dk_Widget_InteractScreen, "Dk.Widget.InteractScreen");

	UE_DEFINE_GAMEPLAY_TAG(Dk_Widget_GameMenu, "Dk.Widget.GameMenu");

	// Images
	UE_DEFINE_GAMEPLAY_TAG(Dk_Image_TestImage, "Dk.Image.TestImage");

	// Items
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Dk_Item_Equipment_Weapons_Axe, "Dk.Item.Equipment.Weapons.Axe", "斧类武器"
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Dk_Item_Equipment_Weapons_Sword, "Dk.Item.Equipment.Weapons.Sword", "单手或双手剑"
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Dk_Item_Equipment_Cloaks_RedCloak, "Dk.Item.Equipment.Cloaks.RedCloak", "鲜红色斗篷"
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Dk_Item_Equipment_Cloaks_WarriorCloak, "Dk.Item.Equipment.Cloaks.WarriorCloak", "战士的斗篷"
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Dk_Item_Equipment_Masks_SteelMask, "Dk.Item.Equipment.Masks.SteelMask", "全覆式钢面具"
	);

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Dk_Item_Consumable_Potions_Health_Small, "Dk.Item.Consumable.Potions.Health.Small", "小血瓶"
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Dk_Item_Consumable_Potions_Health_Large, "Dk.Item.Consumable.Potions.Health.Large", "大血瓶"
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Dk_Item_Consumable_Potions_Mana_Small, "Dk.Item.Consumable.Potions.Mana.Small", "小魔瓶"
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Dk_Item_Consumable_Potions_Mana_Large, "Dk.Item.Consumable.Potions.Mana.Large", "大魔瓶"
	);

	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Dk_Item_CraftingMaterial_FireFernFruit, "Dk.Item.CraftingMaterial.FireFernFruit", "火蕨果，辛辣味"
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Dk_Item_CraftingMaterial_LuminDaisy, "Dk.Item.CraftingMaterial.LuminDaisy", "发光雏菊的花瓣"
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Dk_Item_CraftingMaterial_ScorchPetalBlossom, "Dk.Item.CraftingMaterial.ScorchPetalBlossom", "火山土壤中绽放的花"
	);

	// Inventory Item Fragments Tag
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_Grid, "Dk.Inventory.Fragment.Grid");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_Icon, "Dk.Inventory.Fragment.Icon");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_Stackable, "Dk.Inventory.Fragment.Stackable");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_Consumable, "Dk.Inventory.Fragment.Consumable");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_Equipment, "Dk.Inventory.Fragment.Equipment");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_Name, "Dk.Inventory.Fragment.Name");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_ItemType, "Dk.Inventory.Fragment.ItemType");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_ItemDescription, "Dk.Inventory.Fragment.ItemDescription");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_SellValue, "Dk.Inventory.Fragment.SellValue");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_RequiredLevel, "Dk.Inventory.Fragment.RequiredLevel");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_LabeledValue, "Dk.Inventory.Fragment.LabeledValue");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_LabeledValue_Stat_Primary,
	                       "Dk.Inventory.Fragment.LabeledValue.Stat.Primary");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_LabeledValue_Stat_0, "Dk.Inventory.Fragment.LabeledValue.Stat.0");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_LabeledValue_Stat_1, "Dk.Inventory.Fragment.LabeledValue.Stat.1");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_LabeledValue_Stat_2, "Dk.Inventory.Fragment.LabeledValue.Stat.2");

	// Item Entry Tag
	UE_DEFINE_GAMEPLAY_TAG(Dk_Item_Entry_Attack_Percent, "Dk.Item.Entry.Attack.Percent");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Item_Entry_CritRate, "Dk.Item.Entry.CritRate");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Item_Entry_CritDamage, "Dk.Item.Entry.CritDamage");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Item_Entry_ExecutionChance, "Dk.Item.Entry.ExecutionChance");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Item_Entry_PunctureResistance, "Dk.Item.Entry.PunctureResistance");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Item_Entry_ChopResistance, "Dk.Item.Entry.ChopResistance");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Item_Entry_MaxHealth_Percent, "Dk.Item.Entry.MaxHealth.Percent");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Item_Entry_MonsterExperienceAcquisition, "Dk.Item.Entry.MonsterExperienceAcquisition");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Item_Entry_ShopSellingPrice, "Dk.Item.Entry.ShopSellingPrice");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Item_Entry_EnergyRecoveryBonus, "Dk.Item.Entry.EnergyRecoveryBonus");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Item_Entry_AdrenalineRecoveryBonus, "Dk.Item.Entry.AdrenalineRecoveryBonus");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Item_Entry_WeightLimit, "Dk.Item.Entry.WeightLimit");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Item_Entry_SpellPower, "Dk.Item.Entry.SpellPower");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Item_Entry_MaxToxin, "Dk.Item.Entry.MaxToxin");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Item_Entry_MaxAdrenaline, "Dk.Item.Entry.MaxAdrenaline");
}
