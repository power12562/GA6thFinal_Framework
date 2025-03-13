#pragma once
#include "UmFramework.h"

enum class eDockSpaceArea
{
    LEFT,
    RIGHT,
    UP,
    DOWN,
    SIZE,
};

// 에디터 탭이 윈도우 전체영역에도 도킹이 되도록 글로벌(?) 도킹 영역을 설정
class EditorDockSpace 
    : public EditorWindow
{
public:
    EditorDockSpace();
    virtual ~EditorDockSpace();
private:
    virtual void _CALLBACK OnGuiStart() override;

    virtual void _CALLBACK OnPreFrame() override;
    
    virtual void _CALLBACK OnFrame() override;

    virtual void _CALLBACK OnPostFrame() override;
private:
    /* DockSpace 초기 창 위치 복원 */
    inline void ResetDockBuild() {ImGui::LoadIniSettingsFromMemory(mDockAreaInitalData);  }
    /* DockSpace가 윈도우 전체를 차지하도록 설정함. */
    inline void SetFullSpace(bool isFullSpace) { mIsFullSpace = isFullSpace; }
    /* Dock에 대한 플래그 설정 */
    inline void SetDockFlag(ImGuiDockNodeFlags flag) { mDockNodeFlags = flag; }

    /* 메인 DockSpace의 ID 반환 */
    inline ImGuiID GetDockSpaceID() { return mDockSpaceMainID; }
    /* 메인 DockSpace의 특정 영역 ID 반환 */
    inline ImGuiID GetDockSpaceAreaID(eDockSpaceArea area) { return mDockSpaceAreaID[(INT)area]; }
private:
    /* Dock를 사용할 에디터 목록 */
    std::vector<EditorObject*> mEditorWindowArray;

    /* DockSpace가 화면 전체를 차지하는지 여부 */
    bool mIsFullSpace = true;

    /* DockSpace 플래그 값 */
    ImGuiDockNodeFlags mDockNodeFlags;

    /* 메인 도킹영역 ID값 */
    ImGuiID mDockSpaceMainID;

    /* 도킹 영역에 대한 ID값 */
    ImGuiID mDockSpaceAreaID[(INT)eDockSpaceArea::SIZE];

    /* 초기 도킹 세팅 저장 값 */
    const char* mDockAreaInitalData;
private:
    void InitDockSpaceArea();
};