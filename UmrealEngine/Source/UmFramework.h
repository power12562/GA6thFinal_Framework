#pragma once

#define _CALLBACK

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
#include <string_view>
#include <limits>

#include <vector>
#include <map>

//ThirdParty
#define IMGUI_DEFINE_MATH_OPERATORS
#include "Engine/Imgui/imgui.h"
#include "Engine/Imgui/imgui_stdlib.h"
#include "Engine/Imgui/imgui_impl_win32.h"
#include "Engine/Imgui/imgui_impl_dx11.h"

#include "Engine/Imgui/ImGuizmo.h"
#include "Engine/Imgui/ImSequencer.h"
#include "Engine/Imgui/ImZoomSlider.h"
#include "Engine/Imgui/ImCurveEdit.h"
#include "Engine/Imgui/GraphEditor.h"

//Core
#include "Engine/Core/TimeSystem.h"

#include "Engine/WindowAppCore/SimpleWindowClient.h"

#include "Engine/Framework/SceneManager.h"
#include "Engine/Framework/Scene.h"

// Editor
#include "Engine/Editor/EditorEnum.h"
#include "Engine/Editor/EditorManager.h"
#include "Engine/Editor/EditorBase.h"

//Utility
#include "Engine/Utility/DumpUtility.h"


