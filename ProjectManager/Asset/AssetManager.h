#pragma once

/* 
에셋 매니저는 에셋 폴더를 순회하여 각 MetaData를 읽어와 AssetID를 등록해준다.
AssetID를 통해 해당 에셋의 경로와 실제 데이터를 받아올 수 있음.
또한 AssetSetting을 저장하고 불러온다.
*/

class AssetManager
{
public:
    /* 에셋 정보를 만듭니다. */
    static void Create();
    /* 에셋 정보를 저장합니다. */
    static void Save();
    /* 에셋 정보를 불러옵니다. */
    static void Load();
};