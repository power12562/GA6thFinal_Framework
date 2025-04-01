#include "pch.h"
#include "File.h"

HRESULT File::CreateFileID(ID* _pID)
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
        *_pID = charID;
        return S_OK;
    }
}

void File::CreateFolder(Path _path)
{
    if (!fs::exists(_path))
    {
        if (fs::create_directory(_path))
        {
            std::cout << "Folder created: " << _path << std::endl;
        }
        else 
        {
            std::cerr << "Failed to create folder: " << _path << std::endl;
        }
    }
    else 
    {
        std::cout << "Folder already exists: " << _path << std::endl;
    }
}

bool File::OpenFileNameBrowser(TCHAR* _filter, File::Path& _return)
{
    OPENFILENAME OFN;
    TCHAR filePathName[MAX_PATH] = L"";
    TCHAR lpstrFile[MAX_PATH] = L"";

    memset(&OFN, 0, sizeof(OPENFILENAME));
    OFN.lStructSize = sizeof(OPENFILENAME);
    OFN.hwndOwner = GetFocus();
    OFN.lpstrFilter = _filter;
    OFN.lpstrFile = lpstrFile;
    OFN.nMaxFile = 100;
    OFN.lpstrInitialDir = L".";

    if (GetOpenFileName(&OFN) != 0)
    {
        ProjectManager::Load(OFN.lpstrFile);
    }
    return false;
}

bool File::OpenForderBrowser(TCHAR* _title, UINT _flags, File::Path& _return)
{
    BROWSEINFO bi = { 0 };
    bi.lpszTitle = _title;
    bi.ulFlags = _flags;

    // 폴더 선택 대화상자를 여는 함수
    LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
    if (pidl != NULL) {
        // 선택한 폴더 경로를 얻음
        wchar_t path[MAX_PATH];
        if (SHGetPathFromIDList(pidl, path))
        {
            _return = path;
            return true;
        }
    }
    return false;
}