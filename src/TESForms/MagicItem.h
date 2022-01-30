#pragma once
#include "RE/Skyrim.h"
#include "ExtraInfoEntry.h"

//MagicItem data is found in several forms such as spells and contains a set of magic effects
void GetMagicItemData(ExtraInfoEntry* resultArray, RE::TESForm* baseForm);
void GetEffectData(ExtraInfoEntry* resultArray, RE::Effect* effect, std::string effectActiveString, priority priorityToUse);
