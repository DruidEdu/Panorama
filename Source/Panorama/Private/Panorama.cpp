/* Copyright 2011-2018 Dodgee Software All Rights Reserved. 
	Author: Shem Taylor */

#include "Panorama.h"

#define LOCTEXT_NAMESPACE "FPanoramaModule"

void FPanoramaModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FPanoramaModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FPanoramaModule, Panorama)