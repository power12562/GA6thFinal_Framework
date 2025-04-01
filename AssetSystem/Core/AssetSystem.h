#pragma once

namespace Asset
{
    /*
    에셋 시스템은 에셋 폴더를 순회하여 각 MetaData를 읽어와 AssetID를 등록해준다.
    AssetID를 통해 해당 에셋의 경로와 실제 데이터를 받아올 수 있음.
    */
    class System
    {
    public:
        System();
        ~System();
    private:
        /* 폴더를 순회하며 파일 탐색 */
        static void ReadDiectory(const Path& _path);

        /* 해당 파일을 읽기 시도 */
        static void ReadFile(const Path& _path);

        /* Asset을 등록하여 테이블에 넣음 */
        static void RegisterAsset(const AssetMeta& _metaData);
    public:
        /* 에셋 루트 폴더를 반환 */
        static const Path& GetRootAssetPath();

        /* 메타 파일 확장자를 반환 */
        static const Path& GetMetaExtension();

        /* 메타 파일인지 확인 */
        static bool IsMetaFile(Path _path);

        /* ID가 등록되어 있는지 확인 */
        static bool IsExistID(const AssetID& _assetID);

        static AssetID GetAssetIDWithPath(const Path& _path);
    private:
        /* 에셋을 관리할 루트 경로 */
        static Path mRootPath;
        /* 메타파일 확장자 */
        static Path mMetaExt;

        /* ID to Asset 매핑 테이블 */
        static std::unordered_map <AssetID, Data*> mIDToDataTable;
        static std::unordered_map <Path, AssetID> mPathToIDTable;
    };

    class FileSystem
    {
    public:
        static void CreateFolder(Path _path);
    };
}