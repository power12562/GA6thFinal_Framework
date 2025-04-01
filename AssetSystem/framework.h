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

namespace fs = std::filesystem;
namespace Asset
{
    using Path = fs::path;
    using AssetID = std::string;
};

#include "Extra/AssetMetaData.h"
#include "Extra/AssetData.h"
#include "Core/AssetSystem.h"