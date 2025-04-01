#pragma once

class File
{
public:
    using ID = std::string;
    using Path = std::filesystem::path;
public:
    /* File에 부여할 ID를 생성해줍니다. */
    static HRESULT CreateFileID(ID* _pID);
    /* 해당 경로에 폴더를 만듭니다. */
    static void CreateFolder(Path _path);
    /* 파일열기창을 통해 파일 경로를 받아옵니다. */
    static bool OpenFileNameBrowser(TCHAR* _pFilter, File::Path& _return);
    /* 폴더선택창을 통해 폴더 경로를 받아옵니다. */
    static bool OpenForderBrowser(TCHAR* _title, UINT _flags, File::Path& _return);
};