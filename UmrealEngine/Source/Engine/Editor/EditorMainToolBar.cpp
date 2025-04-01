#include "pch.h"
#include "EditorMainToolBar.h"

EditorMainToolBar::EditorMainToolBar()
{
}

EditorMainToolBar::~EditorMainToolBar()
{
}

void EditorMainToolBar::OnDrawGui()
{
    ImGui::BeginMainMenuBar();

    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New Project...", "Ctrl+Alt+N"))
        {
            NewProject();
        }
        if (ImGui::MenuItem("Save Project...", "Ctrl+Alt+S"))
        {
            SaveProject();
        }
        if (ImGui::MenuItem("Open Project...", "Ctrl+Alt+O"))
        {
            OpenProject();
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
            //Application.RunEnd();
        }

        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Window"))
    {
        if (ImGui::BeginMenu("Tools", "-"))
        {
            for (auto& [key, tool] : Editor.GetRefToolArray())
            {
                bool active = tool->GetActive();
                if (ImGui::MenuItem((key + "##123").c_str(), "-", active))
                {
                    tool->SetActive(active ? false : true);
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

    ImGui::EndMainMenuBar();
}

void EditorMainToolBar::NewProject()
{
    TCHAR title[] = L"폴더를 선택하세요.";
    UINT flags = BIF_USENEWUI | BIF_RETURNONLYFSDIRS;
    File::Path path;
    if (File::OpenForderBrowser(title, flags, path))
    {
        ProjectManager::Create(path);
    }
}

void EditorMainToolBar::SaveProject()
{
    ProjectManager::Save();
}

void EditorMainToolBar::OpenProject()
{
    TCHAR filter[] = L"모든 파일\0*.*\0프로젝트 파일 (.PROJECT)\0*.PROJECT";
    File::Path path;
    if (File::OpenFileNameBrowser(filter, path))
    {
        ProjectManager::Load(path);
    }
}
