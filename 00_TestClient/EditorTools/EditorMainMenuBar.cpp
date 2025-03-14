#include "EditorMainMenuBar.h"
#include "WindowApp.h"

EditorMainMenuBar::EditorMainMenuBar()
{
    SetLabel("MainMenuBar");
}

EditorMainMenuBar::~EditorMainMenuBar()
{
}

void _CALLBACK EditorMainMenuBar::OnGuiStart()
{
    return void _CALLBACK();
}

void _CALLBACK EditorMainMenuBar::OnPreFrame()
{
    return void _CALLBACK();
}

void EditorMainMenuBar::OnFrame()
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Open Project...", "Ctrl+O"))
        {

        }

        ImGui::Separator();

        if (ImGui::MenuItem("New Scene", "Ctrl+N"))
        {

        }

        if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
        {

        }

        if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
        {

        }

        ImGui::Separator();

        if (ImGui::MenuItem("Exit"))
        {
            Application.RunEnd();
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Window"))
    {
        if (ImGui::BeginMenu("Tools", "-"))
        {
            for (auto& [key, tool] : Editor.GetRefToolArray())
            {
                if (tool->GetEditorToolType() != eEditorToolType::WINDOW)
                    continue;

                bool active = tool->GetToolActive();
                if (ImGui::MenuItem((key + "##123").c_str(), "-", active))
                {
                    tool->SetToolActive(active ? false : true);
                }
            }
            ImGui::EndMenu();
        }
        ImGui::Separator();

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Editor"))
    {
        if (ImGui::MenuItem("Full Screen", "-"))
        {
            bool isFullScreen = Editor.IsDockPadding();
            isFullScreen ? Editor.SetDockPadding(false) : Editor.SetDockPadding(true);
        }
        if (ImGui::MenuItem("Padding", "-"))
        {
            bool isPadding = Editor.IsDockPadding();
            isPadding ? Editor.SetDockFullSpace(false) : Editor.SetDockFullSpace(true);
        }
        ImGui::Separator();

        ImGui::EndMenu();
    }
}

void _CALLBACK EditorMainMenuBar::OnPostFrame()
{
    return void _CALLBACK();
}
