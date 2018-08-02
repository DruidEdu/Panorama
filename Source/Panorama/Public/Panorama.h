/* Copyright 2011-2018 Dodgee Software All Rights Reserved. 
	Author: Shem Taylor */

#pragma once

#include "CoreMinimal.h"
#include "ModuleManager.h"

class FPanoramaModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};