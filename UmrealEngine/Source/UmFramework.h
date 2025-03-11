#pragma once

//WINDOW SDK
#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>
#include <directxtk/simplemath.h>
#include <dbghelp.h>

//CRT
#include <tchar.h>

//STL
#include <filesystem>
#include <iostream>
#include <exception>
#include <cstdio>
#include <memory>
#include <string>

//ThirdParty
#include "Engine/Imgui/imgui.h"
#include "Engine/Imgui/imgui_stdlib.h"
#include "Engine/Imgui/imgui_impl_win32.h"
#include "Engine/Imgui/imgui_impl_dx11.h"

//Core
#include "Engine/WindowAppCore/SimpleWindowClient.h"

//Utility
#include "Engine/Utility/DumpUtility.h"