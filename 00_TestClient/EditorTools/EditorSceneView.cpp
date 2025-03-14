#include "EditorSceneView.h"

EditorSceneView::EditorSceneView()
{
    SetLabel("SceneView");
    SetInitialDockSpaceArea(eDockSpaceArea::UP);
}

EditorSceneView::~EditorSceneView()
{
}

void _CALLBACK EditorSceneView::OnGuiStart()
{
    return void _CALLBACK();
}

void _CALLBACK EditorSceneView::OnPreFrame()
{
    return void _CALLBACK();
}

void _CALLBACK EditorSceneView::OnFrame()
{
    ImGui::Text("Label: %s", GetLabel().c_str());
    ImGui::Text("Flag: %d", mFlag);
    ImGui::Text("DockID: %d", ImGui::GetWindowDockID());

    return void _CALLBACK();
}

void _CALLBACK EditorSceneView::OnPostFrame()
{
    return void _CALLBACK();
}