#include "pch.h"
#include "AssetSystem.h"

namespace Asset
{
    Path System::mRootPath = "Assets";
    Path System::mMetaExt = ".META";
    std::unordered_map <AssetID, Data*> System::mIDToDataTable;
    std::unordered_map <Path, AssetID> System::mPathToIDTable;

    System::System()
    {
        FileSystem::CreateFolder(mRootPath);
        ReadDiectory(mRootPath);
    }
    System::~System()
    {
    }
    void System::ReadDiectory(const Path& _path)
    {
        if (!fs::is_directory(_path))
        {
            std::cout << "Invalid directory: " << _path << '\n';
            return;
        }

        std::cout << " Read Directory: " << _path << '\n';

        for (const auto& entry : fs::recursive_directory_iterator(_path))
        {
            if (fs::is_regular_file(entry) && entry.path().extension() != mMetaExt)
            {
                ReadFile(entry.path());
            }
            if (fs::is_directory(entry))
            {
                ReadDiectory(entry.path());
            }
        }
    }
    void System::ReadFile(const Path& _path)
    {
        // 파일이 없으면 return;
        if (false == fs::exists(_path))
        {
            std::cout << "Invalid File: " << _path << '\n';
            return;
        }

        std::cout << " Read File: " << _path << '\n';

        AssetMeta metaData;
        Path metaPath = _path.string() + mMetaExt.string();

        /* meta 파일이 있으면 해당 meta파일을 읽고, 없으면 만들어준다. */
        if (false == fs::exists(metaPath))
        {
            metaData.Create(metaPath);
            std::cout << " Create Meta File: " << _path << '\n';
        }
        else
        {
            metaData.Load(metaPath);
            std::cout << " Load Meta File: " << _path << '\n';
        }
        // ID가 등록이 안되어있으면 등록
        if (false == metaData.IsNull() &&
            false == IsExistID(metaData.GetAssetID()))
        {
            RegisterAsset(metaData);
        }
        // meta 로드가 실패하면 return
        else if(true == metaData.IsNull())
        {
            std::cout << "Null Meta File: " << metaPath << '\n';
            return;
        }
    }
    void System::RegisterAsset(const AssetMeta& _metaData)
    {
        Data* assetData = new Data(_metaData);
        mIDToDataTable[_metaData.GetAssetID()] = assetData;
        mPathToIDTable[_metaData.GetAssetPath()] = _metaData.GetAssetID();
    }
    bool System::IsMetaFile(Path _path)
    {
        return _path.extension() == mMetaExt;
    }
    const Path& System::GetRootAssetPath()
    {
        return mRootPath;
    }
    const Path& System::GetMetaExtension()
    {
        return mMetaExt;
    }
    bool System::IsExistID(const AssetID& _assetID)
    {
        auto itr = mIDToDataTable.find(_assetID);
        if (FIND_SUCCESS(itr, mIDToDataTable))
        {
            return true;
        }
        return false;
    }

    AssetID System::GetAssetIDWithPath(const Path& _path)
    {
        auto itr = mPathToIDTable.find(_path);
        if (FIND_SUCCESS(itr, mPathToIDTable))
        {
            return itr->second;
        }
        return AssetID();
    }

    //========================================================
    //====================== FileSystem ======================
    //========================================================
    void FileSystem::CreateFolder(Path _path)
    {
        if (!std::filesystem::exists(_path)) {
            if (std::filesystem::create_directory(_path)) {
                std::cout << "Folder created: " << _path << std::endl;
            }
            else {
                std::cerr << "Failed to create folder: " << _path << std::endl;
            }
        }
        else {
            std::cout << "Folder already exists: " << _path << std::endl;
        }
    }
}