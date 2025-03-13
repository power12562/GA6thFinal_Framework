#include "EditorDockSpace.h"
#include "EditorTools/EditorDebugView.h"
#include "EditorTools/EditorMainMenuBar.h"
#include "EditorTools/EditorHierarchyView.h"
#include "EditorTools/EditorInspectorView.h"
#include "EditorTools/EditorSceneView.h"

EditorDockSpace::EditorDockSpace()
{
    SetLabel("EditorDockSpace");
    SetDockFlag(ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton);
    mEditorWindowArray.push_back(new EditorMainMenuBar);
    mEditorWindowArray.push_back(new EditorDebugView);
    mEditorWindowArray.push_back(new EditorHierarchyView);
    mEditorWindowArray.push_back(new EditorInspectorView);
    mEditorWindowArray.push_back(new EditorSceneView);
}

EditorDockSpace::~EditorDockSpace()
{
}

void EditorDockSpace::OnFrame()
{
    //////////////////////////////////////////
    // 이전 스타일 값 제거
    //////////////////////////////////////////
    ImGui::PopStyleVar();
    if (mIsFullSpace)
        ImGui::PopStyleVar(2);

    //////////////////////////////////////////
    // DockSpace
    //////////////////////////////////////////
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        mDockSpaceMainID = ImGui::GetID("DockSpace");
        InitDockSpaceArea();
        ImGui::DockSpace(mDockSpaceMainID, ImVec2(0.0f, 0.0f), mDockNodeFlags);
    }
    style.WindowMinSize.x = minWinSizeX;

    
}

void EditorDockSpace::OnGuiStart()
{
}

void EditorDockSpace::OnPreFrame()
{
    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
    SetFlag(ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking);
    if (mIsFullSpace)
    {
        AddFlag(
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus
        );

        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    }
    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (mDockNodeFlags & ImGuiDockNodeFlags_PassthruCentralNode)
        mFlag |= ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
}

void EditorDockSpace::OnPostFrame()
{
    //////////////////////////////////////////
    /* ========GUI Drawing======== */
     //////////////////////////////////////////
    for (auto& tool : mEditorWindowArray)
    {
        if (tool)
        {
            tool->DrawGui();
        }
    }
}

void EditorDockSpace::InitDockSpaceArea()
{
    // 딱 한번만 초기 레이아웃
    if (ImGui::DockBuilderGetNode(mDockSpaceMainID) == NULL)
    {
        ImGui::DockBuilderRemoveNode(mDockSpaceMainID);
        ImGui::DockBuilderAddNode(mDockSpaceMainID, mDockNodeFlags); // 새로 추가

        ImGuiID dock_main_id = mDockSpaceMainID; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
        // 왼쪽 20%
        mDockSpaceAreaID[(INT)eDockSpaceArea::LEFT] = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, NULL, &dock_main_id);
        // 오른쪽 20%
        mDockSpaceAreaID[(INT)eDockSpaceArea::RIGHT] = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.2f, NULL, &dock_main_id);
        // 위쪽 25%
        mDockSpaceAreaID[(INT)eDockSpaceArea::UP] = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.25f, NULL, &dock_main_id);
        // 아래쪽 25%
        mDockSpaceAreaID[(INT)eDockSpaceArea::DOWN] = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.25f, NULL, &dock_main_id);
        // 창 배치
        ImGui::DockBuilderDockWindow("HierarchyView", GetDockSpaceAreaID(eDockSpaceArea::RIGHT));
        ImGui::DockBuilderDockWindow("InspectorView", GetDockSpaceAreaID(eDockSpaceArea::LEFT));
        ImGui::DockBuilderDockWindow("Debug", GetDockSpaceAreaID(eDockSpaceArea::DOWN));
        ImGui::DockBuilderDockWindow("SceneView", GetDockSpaceAreaID(eDockSpaceArea::UP));

        ImGui::DockBuilderFinish(mDockSpaceMainID);

        //mDockAreaInitalData = ImGui::SaveIniSettingsToMemory(); // 저장
    }
}
