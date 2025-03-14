#include "EditorDebugView.h"

EditorDebugView::EditorDebugView()
{
    SetLabel("Debug");
    SetInitialDockSpaceArea(eDockSpaceArea::DOWN);
}

EditorDebugView::~EditorDebugView()
{
}

void _CALLBACK EditorDebugView::OnGuiStart()
{
    return void _CALLBACK();
}

void _CALLBACK EditorDebugView::OnPreFrame()
{
    return void _CALLBACK();
}

void _CALLBACK EditorDebugView::OnFrame()
{
    ImGui::InputDouble("time scale", &Time.timeScale);

    ImGui::Text("time : %f", Time.time());
    ImGui::Text("realtimeSinceStartup : %f", Time.realtimeSinceStartup());

    ImGui::Text("frameCount : %llu", Time.frameCount());

    ImGui::Text("FPS : %d", Time.frameRate());
    ImGui::Text("DeltaTime : %f", Time.deltaTime());

    ImGui::Text("unscaledDeltaTime : %f", Time.unscaledDeltaTime());

    ImGui::InputDouble("Fixed Time Step", &Time.fixedTimeStep);
    ImGui::Text("fixedDeltaTime %f", Time.fixedDeltaTime());
    ImGui::Text("fixedUnscaledDeltaTime %f", Time.fixedUnscaledDeltaTime());

    ImGui::InputDouble("maximumDeltaTime", &Time.maximumDeltaTime);

    ImGui::Text("Label: %s", GetLabel().c_str());
    ImGui::Text("Flag: %d", mFlag);
    ImGui::Text("DockID: %d", ImGui::GetWindowDockID());
}

void _CALLBACK EditorDebugView::OnPostFrame()
{
    return void _CALLBACK();
}