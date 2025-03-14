#include "pch.h"
#include "EditorManager.h"
#include "EditorBase.h"

EditorManager EditorManager::Editor;
EditorManager& Editor = EditorManager::Editor;

EditorManager::EditorManager()
{
    mDockNodeFlags = ImGuiDockNodeFlags_NoCloseButton | ImGuiDockNodeFlags_NoWindowMenuButton;
}

EditorManager::~EditorManager()
{
}

void EditorManager::InitImGuiThemeStyle()
{
    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

    // Headers
    colors[ImGuiCol_Header] = ImVec4{ 0.1f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
    colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
    colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
    colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
    colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
}

void EditorManager::Begin()
{
    ImGuiIO& io = ImGui::GetIO();

    //ImGui::SetCurrentContext(mGuiContext);
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    //ImGuizmo::BeginFrame();

    //////////////////////////////////////////////
    // DockSpace 
    //////////////////////////////////////////////
    UpdateWindowFlag();     // DockWindow Flag
    PushDockStyle();        // Dock Style Push
    BeginDock();            // DockWindow Begin
    SubmitDockSpace();      // Submit Dock
    PopDockStyle();         // Dock Style Pop
}

void EditorManager::End()
{
    EndDock();              // DockWindow End

    ImGuiIO& io = ImGui::GetIO();
    //ImGuiIO& io = ImGui::GetIO();
    //io.DisplaySize = ImVec2((float)application.GetWindow().GetWidth()
    //    , (float)application.GetWindow().GetHeight());

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void EditorManager::OnGuiStart()
{
    for (auto& [key, tool] : mEditorToolArray)
    {
        if (tool)
        {
            tool->OnGuiStart();
        }
    }
}

void EditorManager::DrawGui()
{
    // 비어있을 시 return
    if (mEditorToolArray.empty())
        return;

    Begin();

    /* ========GUI Drawing======== */ 
    for (auto& [key, tool] : mEditorToolArray)
    {
        if (tool)
        {
            ImGui::PushID(reinterpret_cast<uintptr_t>(tool));
            tool->DrawGui();
            ImGui::PopID();
        }
    }
    /* =========================== */

    End();
}

void EditorManager::PushTool(EditorTool* tool)
{
    mEditorToolArray.insert(make_pair(tool->GetLabel(), tool));
}

void EditorManager::InitDockSpaceArea()
{
    if (NULL == ImGui::DockBuilderGetNode(mDockSpaceMainID))
    {
        ImGui::DockBuilderRemoveNode(mDockSpaceMainID);
        ImGui::DockBuilderAddNode(mDockSpaceMainID, mDockNodeFlags); // 새로 추가

        ImGuiID dock_main_id = mDockSpaceMainID; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
        // 오른쪽 20%
        mDockSpaceAreaID[(INT)eDockSpaceArea::RIGHT] = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.25f, NULL, &dock_main_id);
        // 아래쪽 25%
        mDockSpaceAreaID[(INT)eDockSpaceArea::DOWN] = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.40f, NULL, &dock_main_id);
        // 왼쪽 20%
        mDockSpaceAreaID[(INT)eDockSpaceArea::LEFT] = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.30f, NULL, &dock_main_id);
        // 위쪽 25%
        mDockSpaceAreaID[(INT)eDockSpaceArea::UP] = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Up, 0.50f, NULL, &dock_main_id);
        // 나머지 가운데
        mDockSpaceAreaID[(INT)eDockSpaceArea::CENTER] = dock_main_id;

        for (auto& [key, tool] : mEditorToolArray)
        {
            if (tool && tool->GetInitialDockSapceArea() != eDockSpaceArea::NONE)
            {
                ImGui::DockBuilderDockWindow(
                    tool->GetLabel().c_str(),
                    GetDockSpaceAreaID(tool->GetInitialDockSapceArea())
                );
            }
        }
        ImGui::DockBuilderFinish(mDockSpaceMainID);
    }
}

void EditorManager::SubmitDockSpace()
{
    //////////////////////////////////////////
    // Sumit the DockSpace
    //////////////////////////////////////////
    ImGuiIO& io = ImGui::GetIO();
    ImGuiStyle& style = ImGui::GetStyle();
    float minWinSizeX = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        mDockSpaceMainID = ImGui::GetID("MainDockSpace");
        InitDockSpaceArea();
        ImGui::DockSpace(mDockSpaceMainID, ImVec2(0.0f, 0.0f), mDockNodeFlags);
    }
    style.WindowMinSize.x = minWinSizeX;
}

void EditorManager::BeginDock()
{
    ImGui::Begin(mDockingWindowLabel.c_str(), nullptr, mDockWindowFlag);
}

void EditorManager::EndDock()
{
    ImGui::End();
}

void EditorManager::UpdateWindowFlag()
{
    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    mDockWindowFlag = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (mIsFullSpace)
    {
        mDockWindowFlag |=
            ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus |
            ImGuiWindowFlags_NoNavFocus;
    }
    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
    if (mDockNodeFlags & ImGuiDockNodeFlags_PassthruCentralNode)
        mDockWindowFlag |= ImGuiWindowFlags_NoBackground;
}

void EditorManager::PushDockStyle()
{
    if (mIsFullSpace)
    {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    }
    if (!mIsPadding)
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    }
}

void EditorManager::PopDockStyle()
{
    if (mIsFullSpace)
        ImGui::PopStyleVar(2);

    if (!mIsPadding)
        ImGui::PopStyleVar();
}
