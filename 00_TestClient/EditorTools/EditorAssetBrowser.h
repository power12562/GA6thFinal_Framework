#pragma once
#include "UmFramework.h"

// 파일 및 폴더 구조 정의
struct FileNode {
    std::string name;
    bool isFolder;
    std::vector<FileNode> children;
};

class EditorAssetBrowser
    : public EditorTool
{
    enum eShowType
    {
        List,
        Icon,
    };
    enum eFlag
    {
        ShowMetaFile,
        FlagSize,
    };
public:
    EditorAssetBrowser();
    virtual ~EditorAssetBrowser();
private:
    virtual void _CALLBACK OnGuiStart() override;

    virtual void _CALLBACK OnPreFrame() override;

    virtual void _CALLBACK OnFrame() override;

    virtual void _CALLBACK OnPostFrame() override;
private:
    void ShowFolderHierarchy(const Asset::Path& folderPath);

    void ShowFolderContents();

    void ShowContentsToList();
    void ShowContentsToIcon();

    void BeginColum();
    void EndColum();
private:
    /* 브라우저에서 보여질 유형 (List, Icon) */
    eShowType mShowType;
    /* 현재 포커싱 폴더 */ 
    Asset::Path mFocusFolderPath;
    /* 현재 선택된 폴더 or 파일 */
    Asset::Path mSelectedEntry;
    /* 현재 선택된 파일의 ID */
    Asset::AssetID mFocusAssetID;

    /* 패널 위치 저장용 */
    float mPanelWidth = 200.0f;

    /* 각종 플래그 */
    std::array<bool, FlagSize> mAssetBrowserFlags = {false, };
};