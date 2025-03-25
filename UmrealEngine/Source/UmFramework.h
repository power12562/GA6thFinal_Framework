#pragma once
#define _CRT_SECURE_NO_WARNINGS 1

//constexpr
#ifdef UMREALSCRIPTS_EXPORT
#define SCRIPTS_PROJECT true;
constexpr bool IS_SCRIPTS_PROJECT = true;
#else
constexpr bool IS_SCRIPTS_PROJECT = false;
#endif 

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
#include <concepts>
#include <queue>
#include <stack>

//ThirdParty
#define IMGUI_DEFINE_MATH_OPERATORS
#include "Engine/Imgui/imgui.h"
#include "Engine/Imgui/imgui_stdlib.h"
#include "Engine/Imgui/imgui_impl_win32.h"
#include "Engine/Imgui/imgui_impl_dx11.h"
#include "Engine/Imgui/imgui_impl_dx12.h"
#include "Engine/Imgui/imgui_node_editor.h"
#include "Engine/Imgui/imgui_internal.h"

//Utility
#include "Engine/Utility/utfHelper.h"
#include "Engine/Utility/DumpUtility.h"
#include "Engine/Utility/dllUtility.h"

//Class Core
#include "Engine/ClassCore/TProperty.hpp"
#include "Engine/ClassCore/container_traits.h"
#include "Engine/ClassCore/ReflectHelper.h"

//EngineCore Core
#include "Engine/EngineCore/TimeSystem.h"
#include "Engine/EngineCore/GameObjectFactory.h"
#include "Engine/EngineCore/ComponentFactory.h"
#include "Engine/EngineCore/SceneManager.h"
#include "Engine/WindowAppCore/SimpleWindowClient.h"
#include "Engine/EngineCore/EngineCores.h" 

//Game Core
#include "Engine/GameCore/Transform/Transform.h"
#include "Engine/GameCore/GameObject/GameObject.h"
#include "Engine/GameCore/Component/Component.h"

