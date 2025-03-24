﻿#pragma once
class GameObject;

template<typename T>
concept IS_BASE_GAMEOBJECT_C = std::is_base_of_v<GameObject, T>;

class EGameObjectFactory
{
private:
    EGameObjectFactory();
    ~EGameObjectFactory();
public:
    //엔진 접근용 네임스페이스
    struct Engine
    {
        inline static EGameObjectFactory& GetInstance() 
        { 
            static EGameObjectFactory instance;
            return instance; 
        }
    };

    /// <summary>
    /// 게임 오브젝트 클래스를 팩토리에서 생성 가능하도록 등록합니다.
    /// </summary>
    /// <typeparam name="TGameObject :">등록할 타입 파라미터</typeparam>
    template<IS_BASE_GAMEOBJECT_C TGameObject>
    void RegisterGameObject();

    /// <summary>
    /// 게임 오브젝트를 생성합니다. 생성된 오브젝트는 자동으로 씬에 등록됩니다.
    /// </summary>
    /// <param name="typeid_name :">생성할 오브젝트 typeid().name()</param>
    /// <param name="name :">오브젝트 이름</param>
    std::shared_ptr<GameObject> NewGameObject(
        std::string_view typeid_name,
        std::wstring_view name);

private:
    //컴포넌트를 동적할당후 shared_ptr로 반환합니다.
    //매개변수로 생성할 컴포넌트 typeid().name()을 전달해야합니다.
    std::shared_ptr<GameObject> MakeGameObject(std::string_view typeid_name);

    //게임오브젝트를 엔진에 사용하기 위해 초기화합니다.
    void ResetGameObject(
        GameObject* ownerObject,
        std::wstring_view name);
    

private:
    std::map<std::string, std::function<GameObject* ()>> m_NewGameObjectFuncMap;    //생성용 맵
    std::vector<std::string>                             m_NewGameObjectKeyVec;     //키 항목 모음

    //인스턴스 아이디를 추적하기 위한 맴버
    struct
    {
        int backID = 0;
        std::queue<int> emptyID;
    }
    InstanceIDManager;
};

template<IS_BASE_GAMEOBJECT_C TGameObject>
inline void EGameObjectFactory::RegisterGameObject()
{
    const char* key = typeid(TGameObject).name();
    auto findIter = m_NewGameObjectFuncMap.find(key);
    if (findIter == m_NewGameObjectFuncMap.end())
    {
        m_NewGameObjectFuncMap[key] = []
            {
                return new TGameObject;
            };
        m_NewGameObjectKeyVec.emplace_back(key);
    }
    else
    {
        assert(!"This key is already registered.");
    }
}
