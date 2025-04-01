#include "pch.h"
#include "AssetMetaData.h"


namespace Asset
{
    HRESULT AssetMeta::Create(const Path& _path)
    {
        HRESULT hr = CreateAssetID(mAssetID);
        if (FAILED(hr))
        {
            return hr;
        }
          
        std::ofstream fout(_path);
        if (false == fout.is_open()) {
            std::cerr << "Failed to open file: " << _path << std::endl;
            return E_FAIL;
        }
        else
        {
            YAML::Node node;
            node["AssetID"] = mAssetID;
            fout << node;
            mAssetPath = _path;
            mIsNull = false;
            return S_OK;
        }
    }

    HRESULT AssetMeta::Load(const Path& _path)
    {
        YAML::Node node = YAML::LoadFile(_path.string());
        if (node.IsNull())
        {
            return E_FAIL;
        }

        if (node["AssetID"])
        {
            mAssetID = node["AssetID"].as<std::string>();
            mAssetPath = _path;
            mIsNull = false;
            return S_OK;
        }
        return E_FAIL;     
    }
    Path AssetMeta::ToMetaPath(const Path& _path)
    {
        return _path.string() + System::GetMetaExtension().string();
    }

    HRESULT CreateAssetID(AssetID& _id)
    {
        RPC_STATUS status;
        UUID uuid;
        const char* charID;

        status = UuidCreate(&uuid);
        if (RPC_S_OK != status) 
        {
            std::cout << "Failed To UuidCreate(): " << status << '\n';
            return E_FAIL;
        }
        
        status = UuidToStringA(&uuid, (RPC_CSTR*)&charID);
        if (RPC_S_OK != status) 
        {
            std::cout << "Failed To UuidToStringA(): " << status << '\n';
            return E_FAIL;
        }
        else
        {
            _id = charID;
            return S_OK;
        }
    }
}
