#pragma once

#include "MICScaleform_RetrieveExtraData.h"
#include "Util/ScaleformUtil.h"
#include "globals.h"

//4-23-2022: Checked for translations needed

void MICScaleform_RetrieveExtraData::Call(Params& a_params)
{
	REX::DEBUG("RetrieveExtraData: Invoke Start, NumArgs {}", IntToString(a_params.argCount));

	RE::GFxMovie* movie = a_params.movie;
	RE::GFxValue* indexArray = &a_params.args[0];

	ExtraInfoEntry* extrainfoEntryToRetrieve = TraverseExtraInfoEntries(&MICGlobals::rootEntry, indexArray, 0);

	if (extrainfoEntryToRetrieve)
	{

		RE::GFxValue resultArray;
		extrainfoEntryToRetrieve->CreatePrimaryScaleformArray(&resultArray, movie);

		//Returning the desired results can crash the game if the method called takes too long to return the value. Invoking an method in the console.swf when we've finished running our code
		//Seems to prevent this crash
		RE::GFxValue root;
		movie->GetVariable(&root, "_root.consoleFader_mc.Console_mc");

		root.Invoke("AddExtraInfo", 0, &resultArray, 1);
	}
	else
	{
		REX::INFO("Retrieve extra data failed");
	}

	REX::DEBUG("RetrieveExtraData: End");
}

//Recursively travel through the extra info structure to find the desired entry
ExtraInfoEntry* MICScaleform_RetrieveExtraData::TraverseExtraInfoEntries(ExtraInfoEntry* currentEntry, RE::GFxValue* indexArray, uint32_t currentIndex)
{
	REX::DEBUG("Traverse Current Index {}", IntToString(currentIndex));

	RE::GFxValue indexToSelect;
	indexArray->GetElement(currentIndex, &indexToSelect);

	REX::DEBUG("Traverse Current Index {} indexToSelect {}", std::to_string(currentIndex), std::to_string(indexToSelect.GetNumber()));

	ExtraInfoEntry* nextEntry = currentEntry->GetChild((int)indexToSelect.GetNumber());

	if (currentIndex + 1 == indexArray->GetArraySize()) {
		return nextEntry;
	}

	else {
		return TraverseExtraInfoEntries(nextEntry, indexArray, currentIndex + 1);
	}
}
