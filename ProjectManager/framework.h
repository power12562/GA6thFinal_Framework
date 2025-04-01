#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#include <windows.h>
#include <filesystem>
#include <string>
#include <unordered_map>

#include <rpc.h>
#pragma comment(lib, "Rpcrt4.lib")

#undef max
#undef min
#include <rfl.hpp>
#include <rfl/yaml.hpp>
#include <rfl/json.hpp>

// 파일열기대화상자
#include <commdlg.h>
#include <shlobj.h>

namespace fs = std::filesystem;

#include "FileSystem/File.h"
#include "Project/ProjectManager.h"
#include "Setting/SettingManager.h"

//#include "Extra/AssetMetaData.h"
//#include "Extra/AssetData.h"
//#include "Core/AssetSystem.h"