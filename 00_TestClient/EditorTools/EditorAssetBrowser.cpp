#include "EditorAssetBrowser.h"

EditorAssetBrowser::EditorAssetBrowser()
{
    SetLabel("AssetBrowser");
    SetInitialDockSpaceArea(eDockSpaceArea::DOWN);
}

EditorAssetBrowser::~EditorAssetBrowser()
{
}

void _CALLBACK EditorAssetBrowser::OnGuiStart()
{
    return void _CALLBACK();
}

void _CALLBACK EditorAssetBrowser::OnPreFrame()
{
    return void _CALLBACK();
}

void _CALLBACK EditorAssetBrowser::OnFrame()
{
    ImGui::Text("Label: %s", GetLabel().c_str());
    ImGui::Text("Flag: %d", mFlag);
    ImGui::Text("DockID: %d", ImGui::GetWindowDockID());

    return void _CALLBACK();
}

void _CALLBACK EditorAssetBrowser::OnPostFrame()
{
    return void _CALLBACK();
}
