#pragma once

#include "MICScaleform_CopyToClipboard.h"
#include "Util/GeneralUtil.h"
#include <Windows.h>

//4-23-2022: Checked for translations needed

void MICScaleform_CopyToClipboard::Call(Params& a_params)
{
	const char* str = a_params.args[0].GetString();
	std::size_t len = strnlen_s(str, 255);
	HWND hwnd = GetDesktopWindow();
	OpenClipboard(hwnd);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, len+1);
	memcpy_s(GlobalLock(hg), len+1, str, len+1);
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);

	logger::debug("Copy op:", a_params.args[0].GetString());
}
