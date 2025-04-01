#pragma once

namespace Asset
{
    /* 
    MetaData는 Meta파일을 통해 에셋을 식별할 수 있는 고유한 ID(uuid)를 부여해준다.
    ID로 에셋을 관리하기 때문에 경로가 바뀌어도 에셋의 연결성을 유지하게 해주는 역할을 함.
    따라서 사용자가 에셋을 등록하고 커밋 혹은 게시할 때 Meta파일을 함께 올려야함.
    */

    class AssetMeta
    {
    public:
        HRESULT Create(const Path& _path);
        HRESULT Load(const Path& _path);
    public:
        inline bool IsNull() const { return mIsNull; }
        inline const AssetID& GetAssetID() const { return mAssetID; }
        inline const Path& GetAssetPath() const { return mAssetPath; }
    private:
        bool    mIsNull = true;
        AssetID mAssetID = "";
        Path    mAssetPath;
    public:
        static Path ToMetaPath(const Path& _path);
    };

    HRESULT CreateAssetID(AssetID& _id);
}