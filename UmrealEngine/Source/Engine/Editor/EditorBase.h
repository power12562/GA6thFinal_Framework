#pragma once

/*
외부에서 에디터에 출력하기 위한 인터페이스 객체
GameObject, Asset 등은 이를 상속해 InspectorView 등에 그려질 내용을 구현한다.
*/ 
class IEditorObject
{
public:
    IEditorObject() = default;
    virtual ~IEditorObject() = default;
public:
    /* InspectorView에 SetFocus 될 때 호출 */
    virtual void _CALLBACK OnFocusInspectorView() {};
    /* InspectorView의 Draw단계에 호출 */
    virtual void _CALLBACK OnDrawInspectorView() {};
};

/*
도킹이 가능한 에디터 윈도우를 생성할 수 있는 툴
*/
class EditorTool
{
public:
    EditorTool() = default;
    virtual ~EditorTool() = default;
public:
    virtual void _CALLBACK  OnDrawGui();

    /* ImGui시스템이 초기화된 후 한번 호출 */
    virtual void _CALLBACK  OnGuiStart() {};
protected:
    /* Begin 호출 전에 항상 호출 (Begin성공 유무 상관 X) */
    virtual void _CALLBACK  OnPreFrame() {};

    /* Begin 호출 성공 시 호출 */
    virtual void _CALLBACK  OnFrame() {};

    /* End 호출 후에 항상 호출 (Begin성공 유무 상관 X) */
    virtual void _CALLBACK  OnPostFrame() {};
public:
    /*                      타이틀 설정 (중복되는 이름 없어야함) */
    inline void             SetLabel(std::string_view label) { mLabel = label; }
    inline const auto&      GetLabel() { return mLabel; }
    /*                      Gui 활성화 여부 설정 */
    inline void             SetActive(bool isActive) { mIsToolActive = isActive; }
    inline auto             GetActive() { return mIsToolActive; }
    /*                      초기 도킹 영역을 지정 */
    inline void             SetInitialDockSpaceArea(eDockSpaceArea area) { mInitialDockSpaceArea = area; }
    inline auto             GetInitialDockSapceArea() { return mInitialDockSpaceArea; }
    /*                      플래그 설정 */
    inline void             SetWindowFlag(ImGuiWindowFlags flag) { mFlag = flag; }
    inline void             AddWindowFlag(ImGuiWindowFlags flag) { mFlag |= flag; }
protected:
    std::string             mLabel = "";
    bool                    mIsToolActive = true;
    eDockSpaceArea          mInitialDockSpaceArea = eDockSpaceArea::NONE;
    ImGuiWindowFlags        mFlag = ImGuiWindowFlags_None;
    // + 오프셋 등
};