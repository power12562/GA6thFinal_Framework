#pragma once
#include "ScriptsFramework.h"

#ifdef UMREALSCRIPTS_EXPORT
#define UMREALSCRIPTS_DECLSPEC __declspec(dllexport)
#else
#define UMREALSCRIPTS_DECLSPEC __declspec(dllimport)
#endif

extern "C" UMREALSCRIPTS_DECLSPEC void InitalizeScript(const EngineCores& engineCores);

extern "C" UMREALSCRIPTS_DECLSPEC Component* NewTestScript();