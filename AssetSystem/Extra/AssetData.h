#pragma once

namespace Asset
{
    class Data
    {
    public:
        Data(const AssetMeta& _metaData);
        ~Data();
    public:
        /* 에셋이 메모리상에 로드되어 있는가 */
        inline bool IsLoad() { return mResource != nullptr; }

        /*  */

    private:
        /* Asset이 참조하는 Meta정보 */
        AssetMeta mMetaData;

        /* Asset이 존재하는 경로. 직렬화에는 제외 */
        Path mFilePath;

        /* Asset의 실제 메모리에 올린 데이터 */
        void* mResource;
    };

}