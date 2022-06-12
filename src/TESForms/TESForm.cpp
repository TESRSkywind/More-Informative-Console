#include "TESForm.h"
#include "BGSLocation.h"
#include "BGSTextureSet.h"
#include "EffectSetting.h"
#include "EnchantmentItem.h"
#include "MagicItem.h"
#include "SpellItem.h"
#include "TESAmmo.h"
#include "TESModelTextureSwap.h"
#include "TESObjectARMA.h"
#include "TESObjectARMO.h"
#include "TESObjectCELL.h"
#include "TESObjectCONT.h"
#include "TESObjectREFR.h"
#include "TESObjectWeap.h"
#include "TESRace.h"
#include "Util/NameUtil.h"
#include "Util/ScriptUtil.h"
#include "globals.h"
#include "EditorIDCache.h"
#include "FormExtraInfoCache.h"
#include "TranslationCache.h"

//4-30-2022: Checked for translations needed

bool GetHasSourceFileArray(RE::TESForm* form)
{
	return form->sourceFiles.array;  //Check if the source files array exists
}

std::string GetNthFormLocationName(RE::TESForm* form, uint32_t n)
{
	std::string formName = GetTranslation("$Unknown");

	if (GetHasSourceFileArray(form) && form->sourceFiles.array->size() > n) {
		RE::TESFile** sourceFiles = form->sourceFiles.array->data();
		formName = sourceFiles[n]->fileName;
	}

	return formName;
}

std::string GetFirstFormLocationName(RE::TESForm* form)
{
	return GetNthFormLocationName(form, 0);
}

std::string GetLastFormLocationName(RE::TESForm* form)
{
	std::string formName = GetTranslation("$Unknown");;
	int lastFileIndex = GetNumberOfSourceFiles(form) - 1;
	formName = GetNthFormLocationName(form, lastFileIndex);

	return formName;
}

int GetNumberOfSourceFiles(RE::TESForm* form)
{
	int numberOfFiles = 0;

	if (GetHasSourceFileArray(form)) {
		numberOfFiles = form->sourceFiles.array->size();
	}

	return numberOfFiles;
}

//general wrapper for all get form methods
void GetFormData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm, RE::TESObjectREFR* refForm)
{
	logger::debug(("GetExtraData: Get Form Data Start " + GetFormTypeName((int)baseForm->formType.underlying()) + " " + FormIDToString(baseForm->formID)).c_str());

	auto formExtraInfoCache = FormExtraInfoCache::GetSingleton();
	auto extraInfoEntryCached = formExtraInfoCache->GetExtraInfoEntry(baseForm);

	if (extraInfoEntryCached == nullptr )
	{
		GetCommonFormData(resultArray, baseForm, refForm);
		RE::FormType baseFormType = baseForm->GetFormType();

		if (!MICGlobals::minimizeFormDataRead
			|| ( baseFormType != RE::FormType::NPC
				 && baseFormType != RE::FormType::Race ) ) //for the caster of an effect just getting the common form data should be enough information
		{
			if (refForm != nullptr) 
			{
				GetReferenceFormData(resultArray, refForm);
			}

			if (baseForm != nullptr && baseFormType == RE::FormType::NPC && (refForm == nullptr || refForm->GetFormType() == RE::FormType::ActorCharacter)) {
				logger::debug("GetExtraData: Get Form Data character found");
				GetCharacterData(resultArray, refForm, baseForm);
			}

			else if (baseFormType == RE::FormType::MagicEffect) {
				logger::debug("GetExtraData: Get Form Data magic effect found");
				GetMagicEffectData(resultArray, baseForm);
			}

			else if (baseFormType == RE::FormType::Spell) {
				logger::debug("GetExtraData: Get Form Data spell found");
				GetSpellData(resultArray, baseForm);
			}

			else if (baseFormType == RE::FormType::Armor) {
				logger::debug("GetExtraData: Get Form Data armor found");
				GetArmorData(resultArray, baseForm);
			}

			else if (baseFormType == RE::FormType::Weapon) {
				logger::debug("GetExtraData: Get Form Data Weapon found");
				GetWeaponData(resultArray, baseForm);
			}
			else if (baseFormType == RE::FormType::Ammo) {
				logger::debug("GetExtraData: Get Form Data Ammo found");
				GetAmmoData(resultArray, baseForm);
			}

			else if (baseFormType == RE::FormType::Container) {
				logger::debug("GetExtraData: Get Form Data Container found");
				GetContainerData(resultArray, baseForm);
			}
			else if (baseFormType == RE::FormType::Race) {
				logger::debug("GetExtraData: Get Form Data Race found");
				GetRaceEntry(resultArray, baseForm);
			}

			else if (baseFormType == RE::FormType::TextureSet) {
				logger::debug("GetExtraData: Get Form Data Texture Set found");
				GetTextureSet(resultArray, baseForm);
			}

			else if (baseFormType == RE::FormType::Armature) {
				logger::debug("GetExtraData: Get Form Data ARMA found");
				GetArmaData(resultArray, baseForm);
			}

			else if (baseFormType == RE::FormType::Cell) {
				logger::debug("GetExtraData: Get Form Data CELL found");
				GetCellEntry(resultArray, baseForm);
			}
			else if (baseFormType == RE::FormType::Location) {
				logger::debug("GetExtraData: Get Form Data LCTN found");
				GetLocationEntry(resultArray, baseForm);
			}

			//reset any filtering
			MICGlobals::filterARMAByRace = nullptr;

			formExtraInfoCache->CacheExtraInfoEntry(baseForm, resultArray);
		}
	}
	else
	{
		resultArray->CopyChildren(extraInfoEntryCached);
	}

	logger::debug("GetExtraData: Get Form Data End");
}

//get data common to all form types
void GetCommonFormData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm, RE::TESObjectREFR* refForm)
{
	logger::debug("GetCommonFormData Start");

	std::string name = "";

	//The getName function returns the editor ID for race as that is more useful in identifying the actual race
	//but we want to display the real name when we are getting data for the form
	if (baseForm->formType == RE::FormType::Race) {
		RE::TESRace* race = static_cast<RE::TESRace*>(baseForm);
		if (race) {
			name = race->fullName.c_str();
		}
	}

	else {
		//name
		name = GetName(baseForm, refForm);
	}

	ExtraInfoEntry* nameArray;
	auto nameTranslation = GetTranslation("$Name");

	CreateExtraInfoEntry(nameArray, nameTranslation, name, priority_Name);

	resultArray->PushBack(nameArray);


	if (!MICOptions::DisableEditorIDs)
	{
		auto editorIDCache = EditorIDCache::GetSingleton();

		std::string editorID = editorIDCache->GetEditorID(baseForm);

		if (editorID != "")
		{
			ExtraInfoEntry* editorIDEntry;
			CreateExtraInfoEntry(editorIDEntry, GetTranslation("$EditorID"), editorID, priority_EditorID);
			resultArray->PushBack(editorIDEntry);
		}
	}

	//base formid
	std::string formID = FormIDToString(baseForm->formID);

	//If the base form was found in FF, get the template for that form which is going to be more relevant
	if (baseForm->formType == RE::FormType::NPC && baseForm->formID >= 0xFF000000) {
		logger::debug("Found actor with FF base form");
		auto baseFormNonFF = GetRootTemplate(baseForm);
		formID = FormIDToString(baseFormNonFF->formID);
	}

	ExtraInfoEntry* formIDArray;
	CreateExtraInfoEntry(formIDArray, GetTranslation("$BaseFormID"), formID, priority_FormID);
	resultArray->PushBack(formIDArray);

	//base form type
	std::string baseFormType = GetFormTypeName((int)baseForm->formType.underlying());

	ExtraInfoEntry* formTypeEntry;
	CreateExtraInfoEntry(formTypeEntry, GetTranslation("$BaseType"), baseFormType, priority_FormType);
	resultArray->PushBack(formTypeEntry);

	//ref formid
	if (refForm != nullptr) {
		std::string refFormID = FormIDToString(refForm->formID);

		ExtraInfoEntry* formIDArrayReference;
		CreateExtraInfoEntry(formIDArrayReference, GetTranslation("$RefFormID"), refFormID, priority_FormID);
		resultArray->PushBack(formIDArrayReference);
	}

	//mod location info
	GetFormLocationData(resultArray, baseForm, refForm);

	if (!MICGlobals::minimizeFormDataRead)
	{
		//Model information
		GetModelTextures(resultArray, baseForm);

		//Get scripts
		ExtraInfoEntry* scriptsEntry;
		CreateExtraInfoEntry(scriptsEntry, GetTranslation("$Scripts"), "", priority_Scripts_Scripts);

		GetScripts(scriptsEntry, baseForm, refForm);

		//There's no point showing an empty script entry as having no scripts is the standard and some formtypes can't even have scripts. So check if we found anything
		if (scriptsEntry->HasChildren()) {
			resultArray->PushBack(scriptsEntry);
		}

		else {
			delete (scriptsEntry);  //Free up the memory
		}

		//Enchantments. This will only catch enchantments that are part of the base form. Enchantments added by players will be covered elsewhere in the code
		auto formWithEnchantment = baseForm ? baseForm->As<RE::TESEnchantableForm>() : nullptr;
		if (formWithEnchantment && formWithEnchantment->formEnchanting)
		{
			logger::debug("GetCommonFormData: Checking enchantment");

			auto enchantmentForm = formWithEnchantment->formEnchanting;

			ExtraInfoEntry* enchantmentEntry;
			std::string enchantmentName = GetName(enchantmentForm);

			CreateExtraInfoEntry(enchantmentEntry, GetTranslation("$Enchantment"), enchantmentName, priority_Enchantment);
			GetFormData(enchantmentEntry, enchantmentForm, nullptr);

			if (refForm)
			{
				GetCharge(enchantmentEntry, &(refForm->extraList), formWithEnchantment, nullptr);
			}

			resultArray->PushBack(enchantmentEntry);
		}

		//Keywords
		auto keyWordForm = baseForm ? baseForm->As<RE::BGSKeywordForm>() : nullptr;
		if (keyWordForm)
		{
			GetKeywords(resultArray, keyWordForm);
		}

		//MagicItem
		auto magicItem = baseForm ? baseForm->As<RE::MagicItem>() : nullptr;
		if (magicItem)
		{
			GetMagicItemData(resultArray, magicItem);
		}
	}

	logger::debug("GetCommonFormData: GetCommonFormData End");
}

//get information related to where mods the form is found in
void GetFormLocationData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm, RE::TESForm* refForm)
{
	logger::debug("GetExtraData: GetFormLocationData Start");

	ExtraInfoEntry* formLocationHolder;
	CreateExtraInfoEntry(formLocationHolder, GetTranslation("$FormLocation"), "", priority_FormLocation);

	//this method may be called at a time we only have a base form, and in that case skip anything related to the reform
	if (refForm != nullptr && GetHasSourceFileArray(refForm)) {
		logger::debug("GetExtraData: GetFormLocationData ref mod info found");

		//Reference Form

		std::string refFirstDefinedIn = GetFirstFormLocationName(refForm);
		bool SkyrimESMNotDetectedBug = false;

		//fix for weird bug where refs first defined in Skyrim.Esm aren't always detected properly
		if (((refForm->formID & 0xFF000000) == 0) && refFirstDefinedIn != "Skyrim.esm") {
			refFirstDefinedIn = "Skyrim.esm";
			SkyrimESMNotDetectedBug = true;
		}

		ExtraInfoEntry* referenceDefinedIn;
		CreateExtraInfoEntry(referenceDefinedIn, GetTranslation("$RefDefinedIn"), refFirstDefinedIn, priority_FormLocation_ReferenceDefined);

		formLocationHolder->PushBack(referenceDefinedIn);

		std::string refLastDefinedIn = GetLastFormLocationName(refForm);

		ExtraInfoEntry* referenceLastChangedBy;
		CreateExtraInfoEntry(referenceLastChangedBy, GetTranslation("$RefLastModified"), refLastDefinedIn, priority_FormLocation_ReferenceLastChanged);

		formLocationHolder->PushBack(referenceLastChangedBy);

		ExtraInfoEntry* allModsTouchingReferenceHolder;
		CreateExtraInfoEntry(allModsTouchingReferenceHolder, GetTranslation("$RefFoundIn"), "", priority_FormLocation_ReferenceInMods);

		GetModInfoData(allModsTouchingReferenceHolder, refForm, SkyrimESMNotDetectedBug);

		logger::debug("GetExtraData: Ref Last Modified By " + refLastDefinedIn);

		formLocationHolder->PushBack(allModsTouchingReferenceHolder);
	}
	//Base Form

	logger::debug("GetExtraData: GetFormLocationData at pBaseSection section");

	auto baseFormToCheck = baseForm;

	//If the base form was found in FF, get the template for that form which is going to be more relevant
	if (baseForm->formType == RE::FormType::NPC && baseForm->formID >= 0xFF000000) {
		baseFormToCheck = GetRootTemplate(baseForm);
	}

	if (GetHasSourceFileArray(baseFormToCheck)) {
		logger::debug("GetExtraData: GetFormLocationData baseFormModInfo found");

		std::string baseFirstDefinedIn = GetFirstFormLocationName(baseFormToCheck);

		ExtraInfoEntry* baseDefinedIn;
		CreateExtraInfoEntry(baseDefinedIn, GetTranslation("$BaseDefinedIn"), baseFirstDefinedIn, priority_FormLocation_BaseDefined);

		formLocationHolder->PushBack(baseDefinedIn);

		std::string baseLastDefinedIn = GetLastFormLocationName(baseFormToCheck);

		ExtraInfoEntry* baseLastChangedBy;
		CreateExtraInfoEntry(baseLastChangedBy, GetTranslation("$BaseLastModified"), baseLastDefinedIn, priority_FormLocation_BaseLastChanged);

		formLocationHolder->PushBack(baseLastChangedBy);

		ExtraInfoEntry* allModsTouchingBaseHolder;
		CreateExtraInfoEntry(allModsTouchingBaseHolder, GetTranslation("$BaseFoundIn"), "", priority_FormLocation_BaseInMods);

		GetModInfoData(allModsTouchingBaseHolder, baseFormToCheck, false);

		logger::debug("GetExtraData: Base Last Modified By " + baseLastDefinedIn);

		formLocationHolder->PushBack(allModsTouchingBaseHolder);
	}

	resultArray->PushBack(formLocationHolder);

	logger::debug("GetExtraData: GetFormLocationData End");
}

void GetModInfoData(ExtraInfoEntry* resultArray, RE::TESForm* form, bool SkyrimESMNotDetectedBug)
{
	logger::debug("GetExtraData: GetModInfoData start");

	int numMods = GetNumberOfSourceFiles(form);

	if (SkyrimESMNotDetectedBug) {
		ExtraInfoEntry* modEntry;

		CreateExtraInfoEntry(modEntry, GetTranslation("$Mod"), "Skyrim.esm", priority_Default);
		resultArray->PushBack(modEntry);
	}

	for (int i = 0; i < numMods; i++) {
		ExtraInfoEntry* modEntry;

		std::string modName = GetNthFormLocationName(form, i);

		CreateExtraInfoEntry(modEntry, GetTranslation("$Mod"), modName, priority_Default);
		resultArray->PushBack(modEntry);
	}

	logger::debug("GetExtraData: GetModInfoData end");
}

//Get all keywords for forms that store keywords in the normal location
void GetKeywords(ExtraInfoEntry* resultArray, RE::BGSKeywordForm* keywordForm)
{
	logger::debug("GetKeywords Start");

	if (keywordForm) {
		ExtraInfoEntry* keywordsEntry;

		CreateExtraInfoEntry(keywordsEntry, GetTranslation("$Keywords"), "", priority_Keywords);

		for (uint32_t i = 0; i < keywordForm->numKeywords; i++) {
			RE::BGSKeyword* keyword = *(keywordForm->GetKeywordAt(i));

			//if (keyword && ((keyword->formID & 0xFF000000) != 0xFF000000))  //There was a strange Keyword with a formid starting with FF that is causing a crash. There must be some skse plugin reponsible for this
			//																//as it can't be created by normal methods. For now I want to filter those keywords out
			//{
				ExtraInfoEntry* keywordEntry;
				std::string keywordName = GetName(keyword);

				CreateExtraInfoEntry(keywordEntry, keywordName, "", priority_Keywords);

				GetFormData(keywordEntry, keyword, nullptr);

				keywordsEntry->PushBack(keywordEntry);
			//}
		}

		resultArray->PushBack(keywordsEntry);
	}

	logger::debug("GetKeywords End");
}
