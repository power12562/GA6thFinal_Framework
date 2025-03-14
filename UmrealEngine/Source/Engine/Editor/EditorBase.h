#pragma once

// 에디터에 출력이 가능한 객체. GameObject, Asset 등은 이를 상속해 InspectorView에 그려질 내용을 구현한다.
class EditorObject
{
public:
    /* Begin - End 사이에 대한 동작을 구현 */
    virtual void DrawGui() = 0;

    /* ImGui시스템이 초기화된 후 한번 호출 */
    virtual void _CALLBACK OnGuiStart() {};
protected:
    /* Begin 호출 전에 항상 호출 (Begin성공 유무 상관 X) */
    virtual void _CALLBACK OnPreFrame() {};

    /* Begin 호출 성공 시 호출 */
    virtual void _CALLBACK OnFrame() {};

    /* End 호출 후에 항상 호출 (Begin성공 유무 상관 X) */
    virtual void _CALLBACK OnPostFrame() {};
};

class EditorTool
    : public EditorObject
{
public:
    EditorTool();
    virtual ~EditorTool() = default;
public:
    /*                  타이틀 설정 (중복되는 이름 없어야함) */
    inline void         SetLabel(std::string_view label) { mLabel = label; }
    inline const auto&  GetLabel() { return mLabel; }
    /*                  Gui 렌더 여부 설정 */
    inline void         SetToolActive(bool isActive) { mIsToolActive = isActive; }
    inline auto         GetToolActive() { return mIsToolActive; }
    /*                  초기 도킹 영역을 지정 */
    inline void         SetInitialDockSpaceArea(eDockSpaceArea area) { mInitialDockSpaceArea = area; }
    inline auto         GetInitialDockSapceArea() { return mInitialDockSpaceArea; }
    /*                  에디터 툴 타입 */
    inline auto         GetEditorToolType() { return mEditorToolType; }
private:
    std::string     mLabel = "";
    bool            mIsToolActive = true;
    eDockSpaceArea  mInitialDockSpaceArea = eDockSpaceArea::NONE;
protected:
    eEditorToolType mEditorToolType = eEditorToolType::NONE;
    // + 오프셋 등
};

class EditorMenuBar
    : public EditorTool
{
public:
    EditorMenuBar();
    virtual ~EditorMenuBar() = default;
public:
    virtual void DrawGui() final;

    inline void SetInitialDockSpaceArea(eDockSpaceArea area) = delete;
    inline auto GetInitialDockSapceArea() = delete;
};

// 에디터에 출력되는 창
class EditorWindow
    : public EditorTool
{
public:
    EditorWindow();
    virtual ~EditorWindow() = default;
public:
    virtual void DrawGui() final;
public:
    /* 플래그 설정 */
    inline void SetFlag(ImGuiWindowFlags flag) { mFlag = flag; }
    inline void AddFlag(ImGuiWindowFlags flag) { mFlag |= flag; }

    /* Gui 사이즈 설정 */
    inline void SetSize(ImVec2 size) { mWindowSize = size; }
protected:
    ImGuiWindowFlags    mFlag = ImGuiWindowFlags_None;
    ImVec2              mWindowSize = ImVec2(0,0);
};