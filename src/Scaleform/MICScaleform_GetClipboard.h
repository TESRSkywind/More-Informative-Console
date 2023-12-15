#pragma once

#include "RE/Skyrim.h"

class MICScaleform_GetClipboard : public RE::GFxFunctionHandler
{
public:
	virtual void Call(Params& a_params) override;
};

class MICScaleform_LaunchExt : public RE::GFxFunctionHandler
{
public:
	virtual void Call(Params& a_params) override;
};
