#include "pch.h"
#include "EditorManager.h"
#include "EditorBase.h"

EditorManager EditorManager::Editor;
EditorManager& Editor = EditorManager::Editor;

EditorManager::EditorManager()
{
}

EditorManager::~EditorManager()
{
}

void EditorManager::Begin()
{
    //ImGui::SetCurrentContext(mGuiContext);
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    //ImGuizmo::BeginFrame();
}

void EditorManager::End()
{
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
    for (auto& tool : mEditorWindowArray)
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
    if (mEditorWindowArray.empty())
        return;

    Begin();

    /* ========GUI Drawing======== */ 
    for (auto& tool : mEditorWindowArray)
    {
        if (tool)
        {
            tool->DrawGui();
        }
    }
    /* =========================== */

    End();
}

void EditorManager::PushTool(EditorObject* tool)
{
    mEditorWindowArray.push_back(tool);
}
