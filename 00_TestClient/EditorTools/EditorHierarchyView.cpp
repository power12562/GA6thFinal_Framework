﻿#include "EditorHierarchyView.h"

EditorHierarchyView::EditorHierarchyView()
{
    SetLabel("HirearchyView");
}

EditorHierarchyView::~EditorHierarchyView()
{
}

void _CALLBACK EditorHierarchyView::OnGuiStart()
{
    return void _CALLBACK();
}

void _CALLBACK EditorHierarchyView::OnPreFrame()
{
    return void _CALLBACK();
}

void _CALLBACK EditorHierarchyView::OnFrame()
{
    ImGui::Text("Label: %s", mLabel.c_str());
    ImGui::Text("Flag: %d", mFlag);
    ImGui::Text("DockID: %d", ImGui::GetWindowDockID());

    return void _CALLBACK();
}

void _CALLBACK EditorHierarchyView::OnPostFrame()
{
    return void _CALLBACK();
}