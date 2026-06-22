// Fill out your copyright notice in the Description page of Project Settings.


#include "DkGameplayTags.h"

namespace DkGameplayTags
{
	// States
	UE_DEFINE_GAMEPLAY_TAG(Dk_Stats_Health_Full, "Dk.Stats.Health.Full");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Stats_Health_Empty, "Dk.Stats.Health.Empty");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Stats_Dead, "Dk.Stats.Dead");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Stats_Energy_Full, "Dk.Stats.Energy.Full");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Stats_Energy_Empty, "Dk.Stats.Energy.Empty");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Stats_Stun, "Dk.Stats.Stun");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Stats_EquippingSword, "Dk.Stats.EquippingSword");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Stats_Equipped_PrimaryWeapon, "Dk.Stats.Equipped.PrimaryWeapon");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Stats_Sprint, "Dk.Stats.Sprint");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Stats_CanAttack, "Dk.Stats.CanAttack");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Stats_InCombat, "Dk.Stats.InCombat");

	UE_DEFINE_GAMEPLAY_TAG(Dk_Stats_LockingTarget, "Dk.Stats.LockingTarget");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Stats_Crosshair_LockTarget, "Dk.Stats.Crosshair.LockTarget");

	// Ability Tags
	UE_DEFINE_GAMEPLAY_TAG(Dk_Ability_Sprint_Tag, "Dk.Ability.Sprint.Tag");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Ability_LockTarget_Tag, "Dk.Ability.LockTarget.Tag");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Ability_BasicAttack_Tag, "Dk.Ability.BasicAttack.Tag");
	
	// Ability Events
	UE_DEFINE_GAMEPLAY_TAG(Dk_Ability_Passive_HoldSword_Event_Activate, "Dk.Ability.Passive.HoldSword.Event.Activate");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Ability_Passive_Dead_Event_Activate, "Dk.Ability.Passive.Dead.Event.Activate");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Ability_BasicAttack_Event_Damage, "Dk.Ability.BasicAttack.Event.Damage");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Ability_BasicAttack_Event_Change, "Dk.Ability.BasicAttack.Event.Change");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Ability_BasicAttack_Event_Change_End, "Dk.Ability.BasicAttack.Event.Change.End");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Ability_BasicAttack_Event_Change_Combo00, "Dk.Ability.BasicAttack.Event.Change.Combo00");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Ability_BasicAttack_Event_Change_Combo01, "Dk.Ability.BasicAttack.Event.Change.Combo01");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Ability_BasicAttack_Event_Change_Combo02, "Dk.Ability.BasicAttack.Event.Change.Combo02");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Ability_BasicAttack_Event_Change_Combo03, "Dk.Ability.BasicAttack.Event.Change.Combo03");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Ability_BasicAttack_Event_Change_Combo04, "Dk.Ability.BasicAttack.Event.Change.Combo04");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Ability_BasicAttack_Event_Change_Combo05, "Dk.Ability.BasicAttack.Event.Change.Combo05");

	// Input Actions
	UE_DEFINE_GAMEPLAY_TAG(Dk_Input_Action_Move, "Dk.Input.Action.Move");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Input_Action_ToggleMoveStyle, "Dk.Input.Action.ToggleMoveStyle");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Input_Action_Look, "Dk.Input.Action.Look");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Input_Action_Jump, "Dk.Input.Action.Jump");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Input_Action_Interact, "Dk.Input.Action.Interact");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Input_Action_OpenInventory, "Dk.Input.Action.OpenInventory");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Input_Action_OpenSystemMenu, "Dk.Input.Action.OpenSystemMenu");
	
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

	UE_DEFINE_GAMEPLAY_TAG(Dk_Widget_SystemMenu, "Dk.Widget.SystemMenu");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Widget_GameMenu, "Dk.Widget.GameMenu");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Widget_GameMenu_Inventory, "Dk.Widget.GameMenu.Inventory");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Widget_GameMenu_Map, "Dk.Widget.GameMenu.Map");

	// Images
	UE_DEFINE_GAMEPLAY_TAG(Dk_Image_TestImage, "Dk.Image.TestImage");

	// Items
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Dk_Item_Equipment_Weapons_Primary, "Dk.Item.Equipment.Weapons.Primary", "主武器类别"
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Dk_Item_Equipment_Weapons_Primary_Sword, "Dk.Item.Equipment.Weapons.Primary.Sword", "主武器钢剑"
	);
	UE_DEFINE_GAMEPLAY_TAG_COMMENT(
		Dk_Item_Equipment_Weapons_Secondary_Sword, "Dk.Item.Equipment.Weapons.Secondary.Sword", "副武器银剑"
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
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_Entry, "Dk.Inventory.Fragment.Entry");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_Entry_Main_0, "Dk.Inventory.Fragment.Entry.Main.0");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_Entry_Main_1, "Dk.Inventory.Fragment.Entry.Main.1");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_Entry_Sub_0, "Dk.Inventory.Fragment.Entry.Sub.0");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_Entry_Sub_1, "Dk.Inventory.Fragment.Entry.Sub.1");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_Entry_Sub_2, "Dk.Inventory.Fragment.Entry.Sub.2");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Inventory_Fragment_Entry_Sub_3, "Dk.Inventory.Fragment.Entry.Sub.3");

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

	// Action Input Buffer（弃用）
	UE_DEFINE_GAMEPLAY_TAG(Dk_Action_CanCombo, "Dk.Action.CanCombo");
	UE_DEFINE_GAMEPLAY_TAG(Dk_Action_NoAction, "Dk.Action.NoAction");
}
