#pragma once

#include "MICScaleform_GetClipboard.h"
#include "Util/GeneralUtil.h"
#include "Util/ScaleformUtil.h"
#include <Windows.h>
#include <filesystem>

void MICScaleform_GetClipboard::Call(Params& a_params)
{
	std::string text{};
	RE::GFxValue* results = a_params.retVal;
	RE::GFxMovie* movie = a_params.movie;

	// Try opening the clipboard
	if (!IsClipboardFormatAvailable(CF_TEXT) || !OpenClipboard(nullptr))
	{
		return;
	}
	// Get handle of clipboard object for ANSI text
	const HANDLE hData = GetClipboardData(CF_TEXT);
	if (hData != nullptr)
	{
		// Lock the handle to get the actual text pointer
		if (const auto pszText = static_cast<const char*>(GlobalLock(hData)); pszText != nullptr)
		{
			text = pszText;
		}

		// Release the lock
		GlobalUnlock(hData);

		// Release the clipboard
		CloseClipboard();
	}

	movie->CreateString(results, text.c_str());
	logger::debug("Paste op:", text);
}


void MICScaleform_LaunchExt::Call(Params& a_params)
{
	std::filesystem::path relative{ a_params.args[0].GetString() };
	auto fullPath = std::filesystem::current_path();
	if (!relative.has_extension())
		return;
	
	if (relative.extension() == ".nif")
	{
		fullPath /= "Data\\meshes" / relative;
		if (std::filesystem::exists(fullPath))
		{
			ShellExecuteW(0, 0, fullPath.c_str(), 0, 0, SW_SHOW);
		}
	}
}
