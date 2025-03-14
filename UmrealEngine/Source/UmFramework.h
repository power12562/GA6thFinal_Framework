#pragma once

//WINDOW SDK
#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>
#include <directxtk/simplemath.h>
#include <dbghelp.h>
#include <dxgi1_4.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "Dbghelp.lib")

//CRT
#include <tchar.h>

//STL
#include <filesystem>
#include <iostream>
#include <exception>
#include <cstdio>
#include <memory>
#include <string>
#include <limits>

//ThirdParty
#include "Engine/Imgui/imgui.h"
#include "Engine/Imgui/imgui_stdlib.h"
#include "Engine/Imgui/imgui_impl_win32.h"
#include "Engine/Imgui/imgui_impl_dx11.h"

//Class Core
#include "Engine/ClassCore/container_traits.h"
#include "Engine//ClassCore/ReflectHelper.h"
#include "Engine/ClassCore/TProperty.hpp"

//Core
#include "Engine/Core/TimeSystem.h"

#include "Engine/WindowAppCore/SimpleWindowClient.h"

//Utility
#include "Engine/Utility/DumpUtility.h"


