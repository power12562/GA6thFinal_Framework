#pragma once
//스크립트 dll에서 엔진 코어에 접근하기 위한 구조체
struct EngineCores
{
    EngineCores(
        TimeSystem& _Time,
        SceneManager& _sceneManager,
        ComponentFactory& _componentFactory
    )
        :
        Imguicontext(*ImGui::GetCurrentContext()),
        Time(_Time),
        sceneManager(_sceneManager),
        componentFactory(_componentFactory)
    {

    }
    ~EngineCores() = default;

    ImGuiContext& Imguicontext;
    TimeSystem& Time;
    SceneManager& sceneManager;
    ComponentFactory& componentFactory;
};

#ifdef SCRIPTS_PROJECT
extern std::unique_ptr<EngineCores> UmrealEngine; //스크립트에서 엔진 접근을 위한 전역 변수.
#endif


