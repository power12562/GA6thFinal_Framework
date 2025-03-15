#pragma once

//스크립트 dll에서 엔진 코어에 접근하기 위한 구조체
struct EngineCores
{
    EngineCores(
        TimeSystem& timeSystem
    )
        :
        Time(timeSystem),
        Imguicontext(*ImGui::GetCurrentContext())
    {}
    ~EngineCores() = default;

    TimeSystem& Time;
    ImGuiContext& Imguicontext;
};