#pragma once
#include "IncludeScripts.h"

#ifdef UMREALSCRIPTS_EXPORT
#define UMREALSCRIPTS_DECLSPEC __declspec(dllexport)
#else
#define UMREALSCRIPTS_DECLSPEC __declspec(dllimport)
#endif

extern "C" UMREALSCRIPTS_DECLSPEC Component* NewTestScripts();