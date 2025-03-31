#pragma once
//엔진 코어에 접근하기 위한 구조체
class EngineCores
{
public:
    EngineCores();
    ~EngineCores();

    ETimeSystem Time;
    ESceneManager SceneManager;
    EGameObjectFactory GameObjectFactory;
    EComponentFactory ComponentFactory;

public:
    class Engine
    {
        friend class Application;
        /*엔진 코어를 생성합니다. Application에서 호출합니다.*/
        static void CreateEngineCores();

        /*엔진 코어를 파괴합니다. Application에서 호출합니다.*/
        static void DestroyEngineCores();
    };
};

extern std::shared_ptr<EngineCores> EngineCore; //스크립트에서 엔진 접근을 위한 전역 변수.



