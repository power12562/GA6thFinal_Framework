#include "EditorInspectorView.h"

EditorInspectorView::EditorInspectorView()
{
    SetLabel("InspectorView");
    SetInitialDockSpaceArea(eDockSpaceArea::RIGHT);
}

EditorInspectorView::~EditorInspectorView()
{
}

void _CALLBACK EditorInspectorView::OnGuiStart()
{
    return void _CALLBACK();
}

void _CALLBACK EditorInspectorView::OnPreFrame()
{
    return void _CALLBACK();
}

void _CALLBACK EditorInspectorView::OnFrame()
{
    ImGui::Text("Label: %s", GetLabel().c_str());
    ImGui::Text("Flag: %d", mFlag);
    ImGui::Text("DockID: %d", ImGui::GetWindowDockID());

    return void _CALLBACK();
}

void _CALLBACK EditorInspectorView::OnPostFrame()
{
    return void _CALLBACK();
}
