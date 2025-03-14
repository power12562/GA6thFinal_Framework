#include "EditorMainMenuBar.h"
#include "WindowApp.h"

EditorMainMenuBar::EditorMainMenuBar()
{
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
}

void _CALLBACK EditorMainMenuBar::OnPostFrame()
{
    return void _CALLBACK();
}
