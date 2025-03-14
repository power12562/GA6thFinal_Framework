#include "EditorAssetBrowser.h"

EditorAssetBrowser::EditorAssetBrowser()
{
    SetLabel("AssetBrowser");
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
    ImGui::Text("Label: %s", mLabel.c_str());
    ImGui::Text("Flag: %d", mFlag);
    ImGui::Text("DockID: %d", ImGui::GetWindowDockID());

    return void _CALLBACK();
}

void _CALLBACK EditorAssetBrowser::OnPostFrame()
{
    return void _CALLBACK();
}
