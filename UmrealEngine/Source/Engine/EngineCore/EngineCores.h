#pragma once
//스크립트 dll에서 엔진 코어에 접근하기 위한 구조체
class EngineCores
{
public:
    EngineCores(
        TimeSystem& _Time,
        SceneManager& _sceneManager,
        ComponentFactory& _componentFactory,
        GameObjectFactory& _gameObjectFactory
    )
        :
        Imguicontext(*ImGui::GetCurrentContext()),
        Time(_Time),
        sceneManager(_sceneManager),
        componentFactory(_componentFactory),
        gameObjectFactory(_gameObjectFactory)
    {

    }
    ~EngineCores() = default;

    ImGuiContext& Imguicontext;
    TimeSystem& Time;
    SceneManager& sceneManager;
    GameObjectFactory& gameObjectFactory;
    ComponentFactory& componentFactory;
};

#ifdef SCRIPTS_PROJECT
extern std::unique_ptr<EngineCores> UmrealEngine; //스크립트에서 엔진 접근을 위한 전역 변수.
#endif


