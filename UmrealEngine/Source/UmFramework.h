#pragma once

//WINDOW SDK
#include <Windows.h>
#include <wrl.h>
#include <d3d11.h>
#include <dbghelp.h>
#include <dxgi1_4.h>
#include <directxtk/simplemath.h>

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
#include <array>
#include <vector>
#include <type_traits>
#include <functional> 
#include <unordered_map>
#include <any>
#include <typeinfo>
#include <ranges>

//ThirdParty
#include "Engine/Imgui/imgui.h"
#include "Engine/Imgui/imgui_stdlib.h"
#include "Engine/Imgui/imgui_impl_win32.h"
#include "Engine/Imgui/imgui_impl_dx11.h"

//Utility
#include "Engine/Utility/DumpUtility.h"

//Class Core
#include "Engine/ClassCore/TProperty.hpp"
#include "Engine/ClassCore/container_traits.h"
#include "Engine/ClassCore/ReflectHelper.h"

//Engine Core
#include "Engine/EngineCore/TimeSystem.h"

#include "Engine/WindowAppCore/SimpleWindowClient.h"

//Game Core
#include "Engine/GameCore/Component/Component.h"




