#pragma once

//스크립트 dll에서 엔진 코어에 접근하기 위한 구조체
struct EngineCores
{
    EngineCores(
        TimeSystem& _Time,
        SceneManager& _sceneManager
    )
        :
        Imguicontext(*ImGui::GetCurrentContext()),
        Time(_Time),
        sceneManager(_sceneManager)
    {}
    ~EngineCores() = default;

    ImGuiContext& Imguicontext;
    TimeSystem& Time;
    SceneManager& sceneManager;
};