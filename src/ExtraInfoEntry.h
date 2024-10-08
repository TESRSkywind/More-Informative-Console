#pragma once

#include "RE/Skyrim.h"
#include <Vector>

class ExtraInfoEntry
{
public:
	ExtraInfoEntry(std::string entry1, std::string entry2, int priority);

	void Clear();
	void PushBack(ExtraInfoEntry* subArrayEntry);
	void CopyChildren(ExtraInfoEntry* entryToCopyFrom);

	void SetMayCopy(bool valueToSet );

	bool HasChildren();

	ExtraInfoEntry* GetChild(int index);
	void CreatePrimaryScaleformArray(RE::GFxValue* mainScaleFormArray, RE::GFxMovie* root);
	void CreateSecondaryScaleformArray(RE::GFxValue* scaleFormArray, RE::GFxMovie* root);

	void Finalize();

	std::string entry1;
	std::string entry2;

	int priority;  //Priority for use in sorting. Higher Priority entires move to the top of the list
	bool disableSortingByName;

	bool needsExpansion;
	int subarrayCountOverride;
	RE::BSScript::Object* scriptToExpand;

private:

	int parents;
	bool isFinalized;
	bool mayCopy;


	std::vector<ExtraInfoEntry*> subarray;
};

enum priority
{
	priority_Warning,
	//General items at the type of any list
	priority_Name,
	priority_EditorID,
	priority_EditorIDReference,
	priority_FormID,
	priority_FormType,
	priority_FormLocation,  //Mods form is found in
	priority_ExtraData,     //This will only show up in debug mode. Lists all the forms of extra data attached to the reference
	priority_Model,
	priority_Texture,
	priority_TextureSet,
	//Actor-Part1
	priority_Actor_Health,
	priority_Actor_Magicka,
	priority_Actor_Stamina,
	priority_Actor_Level,
	priority_Actor_Race,
	priority_Actor_Voice,
	priority_Actor_Spells,
	priority_Actor_Spells_AddedSpell,
	priority_Actor_Spells_BaseSpell,
	priority_Actor_Effects,
	//Extra Data
	priority_ExtraContainerChanges_Equipment,
	priority_ExtraContainerChanges_Inventory,
	priority_ExtraContainerChanges_Item,
	//Keyword
	priority_Keywords,
	priority_Keyword,
	//Actor-Part 2
	priority_Actor_ActorValues,
	priority_Actor_ActorValues_ActorValue,
	priority_Actor_Perks,
	priority_Actor_Perks_Perk,
	priority_Actor_Protection,
	priority_Actor_IsPCLeveleMult,
	priority_Actor_IsPCLeveleMult_LevelMult,
	priority_Actor_IsPCLeveleMult_LevelMin,
	priority_Actor_IsPCLeveleMult_LevelMax,
	priority_Actor_Factions,
	priority_Actor_Factions_Faction,
	priority_Actor_CurrentPackage,
	priority_Actor_Appearance,
	priority_Actor_Appearance_Weight,
	priority_Actor_Appearance_Height,
	//ActorValue
	priority_ActorValue_Base,
	priority_ActorValue_Current,
	priority_ActorValue_Max,
	//Armature
	priority_Arma_SkinMale,
	priority_Arma_SkinFemale,
	priority_Arma_PrimaryRace,
	priority_Arma_AddtionalRaces,
	//Enchantments
	priority_Enchantment,
	priority_Enchantment_Charge,
	priority_Enchantment_MaximumCharge,
	//Armor
	priority_Armor_ArmorType,
	priority_Armor_ArmorRating,
	priority_Armor_Value,
	priority_Armor_Weight,
	priority_Armor_EquipSlots,
	priority_Armor_ArmorAddons,
	//Ammo
	priority_Ammo_Damage,
	priority_Ammo_Value,
	//Cell
	priority_Cell_Coordinate,
	priority_Cell_Owner,
	//Location
	priority_Location_ParentLocation,
	priority_Location_Cleared,
	//Race
	priority_Race_Skin,
	priority_Race_SkeletonMale,
	priority_Race_SkeletonFemale,
	priority_Race_Child,
	priority_Race_Playable,
	//Reference Specific Items
	priority_Reference_Location,                //XYZ position of reference
	priority_Reference_Enabled,                 //Is the reference enabled or disabled
	priority_Reference_ExtraData_EnableParent,  //Enable Parent Information
	//Spell
	priority_Spell_SpellType,
	priority_Spell_BaseCost,
	priority_Spell_CastTime,
	priority_Spell_DeliveryType,
	//Magic Item (This is used in Spells and ... )
	priority_MagicItem_MagicEffects,
	priority_MagicItem_Effect_Active,
	priority_MagicItem_Effect_Inactive,
	priority_MagicItem_Effect,
	//Effects - This is one half of magic effect data. This is the magnitude/duration/area part
	priority_Effect_Magnitude,
	priority_Effect_Duration,
	priority_Effect_Area,
	priority_Effect_Caster,
	priority_Effect_Active,
	//EffectSetting
	priority_EffectSetting_EffectType,
	priority_EffectSetting_Skill,
	priority_EffectSetting_PrimaryAV,
	priority_EffectSetting_SecondaryAV,
	priority_EffectSetting_Resistance,
	priority_EffectSetting_Hostile,
	priority_EffectSetting_MinimumSkill,
	priority_EffectSetting_DeliveryType,
	//Texture Set
	priority_TextureSet_TextureSet,
	priority_TextureSet_TextureName,
	//Weapon
	priority_Weapon_AnimationType,
	priority_Weapon_Damage,
	priority_Weapon_Speed,
	priority_Weapon_Value,
	priority_Weapon_Weight,
	priority_Weapon_EquipSlotName,
	priority_Weapon_Reach,
	priority_Weapon_Stagger,
	priority_Weapon_CritDamage,
	//Lock data
	priority_ExtraLock_Lock,
	priority_ExtraLock_LockLevel,
	priority_ExtraLock_Key,
	//Container
	priority_Container_IsSafe,
	//Quests
	priority_Quest_Status,
	priority_Quest_CurrentStage,
	//Scripts
	priority_Scripts_Scripts,
	priority_Scripts_Script,
	priority_Scripts_Source,
	priority_Scripts_VariableProperty,
	//Aliases
	priorty_Aliases,
	priorty_Alias,
	priority_Alias_AliasName,
	priority_Alias_AliasType,
	priority_Alias_AliasQuest,
	priority_Alias_AliasTarget,
	//Global
	priorty_Global_Type,
	priorty_Global_Value,
	//Extra Data
	priority_ExtraData_EnableParent_ParentForm,
	priority_ExtraData_EnableParent_StateOppositeParent,
	priority_ExtraData_EnableParent_Popin,
	priority_ExtraData_Ownership,
	priority_ExtraData_Location,
	priority_ExtraData_RegionList,
	priority_ExtraData_Region,
	//Form Location Information
	priority_FormLocation_BaseDefined,
	priority_FormLocation_BaseLastChanged,
	priority_FormLocation_BaseInMods,
	priority_FormLocation_ReferenceDefined,
	priority_FormLocation_ReferenceLastChanged,
	priority_FormLocation_ReferenceInMods,
	//Position
	priority_Position_XCoordinate,
	priority_Position_YCoordinate,
	priority_Position_ZCoordinate,
	priority_Position_XRotation,
	priority_Position_YRotation,
	priority_Position_ZRotation,
	//WorldData Top Level Items
	priority_WorldData_WorldSpace,
	priority_WorldData_Cell,
	priority_WorldData_MusicType,
	priority_WorldData_MusicTrack,
	priority_WorldData_Weather,
	//MFG
	priority_MFG_Expression,
	priority_MFG_Modifier,
	priority_MFG_Phenome,
	//Bottom of the list objects
	priority_Default
};

void CreateExtraInfoEntry(ExtraInfoEntry*& extraInfoEntry, std::string extraInfoName, std::string extraInfoContents, priority priority);

//Enum for priority order Highest up takes precedence in sorting
