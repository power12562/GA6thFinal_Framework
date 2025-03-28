#pragma once
//엔진 코어에 접근하기 위한 구조체
class EngineCores
{
public:
    EngineCores();
    ~EngineCores();
    struct Engine
    {
        static std::unique_ptr<EngineCores> MakeEngineCores();
    };

    ETimeSystem Time;
    ESceneManager SceneManager;
    EGameObjectFactory GameObjectFactory;
    EComponentFactory ComponentFactory;
};

extern std::unique_ptr<EngineCores> EngineCore; //스크립트에서 엔진 접근을 위한 전역 변수.



