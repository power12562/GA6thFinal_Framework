#pragma once

class UID
{
protected:
    UID()
    {
        id = GlobalId++;
    }
    inline static UINT GlobalId = 0;
    UINT id;
};

// 에디터에 출력이 가능한 객체
class EditorObject : public UID
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
    EditorTool() = default;
    virtual ~EditorTool() = default;
public:
    /* Gui 렌더 여부 설정 */
    inline void SetActive(bool isActive) { mIsActive = isActive; }
protected:
    bool    mIsActive = true;
    // + 오프셋 등
};

class EditorMenuBar
    : public EditorTool
{
public:
    using EditorTool::EditorTool;
public:
    virtual void DrawGui() final;
};

// 에디터에 출력되는 창
class EditorWindow
    : public EditorTool
{
public:
    using EditorTool::EditorTool;
public:
    virtual void    DrawGui() final;
public:
    /* 타이틀 설정 (중복되는 이름 없어야함)*/
    inline void     SetLabel(std::string_view label) { mLabel = label; }

    /* 플래그 설정 */
    inline void     SetFlag(ImGuiWindowFlags flag) { mFlag = flag; }
    inline void     AddFlag(ImGuiWindowFlags flag) { mFlag |= flag; }

    /* Gui 사이즈 설정 */
    inline void     SetSize(ImVec2 size) { mWindowSize = size; }

    /* 초기 도킹 영역을 지정 */
    inline void SetDockSpaceArea(eDockSpaceArea area) { mDockSpaceArea = area; }

    inline auto GetDockSapceArea() { return mDockSpaceArea; }
protected:
    std::string         mLabel = "";
    ImGuiWindowFlags    mFlag = ImGuiWindowFlags_None;
    ImVec2              mWindowSize = ImVec2(0,0);
    eDockSpaceArea      mDockSpaceArea;
};