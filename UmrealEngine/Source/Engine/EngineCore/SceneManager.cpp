#include "pch.h"

ESceneManager& SceneManager = ESceneManager::Engine::GetInstance();

void ESceneManager::Engine::SceneUpdate()
{
    SceneManager.SceneUpdate();
}

void ESceneManager::Engine::AddGameObjectToLifeCycle(std::shared_ptr<GameObject> gameObject)
{
    auto [iter, result] = SceneManager.m_runtimeObjectsUnorderedMap[gameObject->m_name].insert(gameObject);
    if (result == false)
    {
        assert(!"이미 추가한 게임 오브젝트 입니다.");
    }
    else
    {
        SceneManager.m_AddGameObjectsQueue.push_back(gameObject);
    }
}

void ESceneManager::Engine::AddComponentToLifeCycle(std::shared_ptr<Component> component)
{
    SceneManager.m_AddComponentsQueue.push_back(component);
}

void ESceneManager::Engine::SetGameObjectActive(int instanceID, bool value)
{
    if (auto& gameObject = SceneManager.m_runtimeObjects[instanceID])
    {
        if (gameObject->m_activeSelf != value)
        {
            gameObject->m_activeSelf = value;

            //컴포넌트들의 On__able 함수를 호출하도록 합니다.
            auto& [WaitSet, WaitVec] = value ? SceneManager.m_OnEnableQueue : SceneManager.m_OnDisableQueue;
            for (auto& component : gameObject->m_components)
            {
                if (component->Enable)
                {
                    auto [iter, result] = WaitSet.insert(component.get());
                    if (result)
                    {
                        WaitVec.push_back(component.get());
                    }
                }
            }
        }
    }
}

void ESceneManager::Engine::SetComponentEnable(Component* component, bool value)
{
    if (component && component->m_enable != value)
    {
        component->m_enable = value;

        //컴포넌트의 On__able 함수를 호출하도록 합니다.
        auto& [WaitSet, WaitVec] = value ? SceneManager.m_OnEnableQueue : SceneManager.m_OnDisableQueue;
        if (component->gameObect->activeSelf)
        {
            auto [iter, result] = WaitSet.insert(component);
            if (result)
            {
                WaitVec.push_back(component);
            }
        }
    }
}

std::weak_ptr<GameObject> ESceneManager::Engine::FindGameObjectWithName(std::wstring_view name)
{
    std::weak_ptr<GameObject> findObject;
    auto findIter = SceneManager.m_runtimeObjectsUnorderedMap.find(name.data());
    if (findIter != SceneManager.m_runtimeObjectsUnorderedMap.end() && !findIter->second.empty())
    {
        findObject = *findIter->second.begin();
    }
    return findObject;
}

void ESceneManager::SceneUpdate()
{
    ETimeSystem::Engine::TimeSystemUpdate();
    ObjectsAwake();
    ObjectsOnEnable();
    ObjectsStart();
    while (ETimeSystem::Engine::TimeSystemFixedUpdate())
    {
        ObjectsFixedUpdate();
    }
    ObjectsUpdate();
    ObjectsLateUpdate();
    ObjectsOnDisable();
    ObjectsAddToLifeCycle();
}

void ESceneManager::ObjectsAwake()
{
    for (auto& component : m_WaitAwakeVec)
    {
        if (component->m_gameObect->activeInHierarchy_property_getter())
        {
            component->Awake();
            if (component->m_enable)
            {
                component->OnEnable();
            }
        }
    }
    std::erase_if(m_WaitAwakeVec, [](auto& component)
        {
            return component->m_gameObect->activeInHierarchy_property_getter();
        });
}

void ESceneManager::ObjectsStart()
{
    for (auto& component : m_WaitStartVec)
    {
        if (component->m_enable)
        {
            component->Start();
        }
    }
    std::erase_if(m_WaitStartVec, [](auto& component)
        {
            return component->m_enable;
        });
}

void ESceneManager::ObjectsFixedUpdate()
{
    for (auto& obj : m_runtimeObjects)
    {
        if (IsRuntimeActive(obj))
        {
            for (auto& component : obj->m_components)
            {        
                if(component->Enable)
                    component->FixedUpdate();
            }
        }
    }
}

void ESceneManager::ObjectsUpdate()
{
    for (auto& obj : m_runtimeObjects)
    {
        if (IsRuntimeActive(obj))
        {
            for (auto& component : obj->m_components)
            {
                if (component->Enable)
                    component->Update();
            }
        }
    }
}

void ESceneManager::ObjectsLateUpdate()
{
    for (auto& obj : m_runtimeObjects)
    {
        if (IsRuntimeActive(obj))
        {
            for (auto& component : obj->m_components)
            {
                if (component->Enable)
                    component->LateUpdate();
            }
        }
    }
}

void ESceneManager::ObjectsOnEnable()
{
    auto& [OnEnableSet, OnEnableVec] = m_OnEnableQueue;
    for (auto& component : OnEnableVec)
    {
        component->OnEnable();
    }
    OnEnableSet.clear();
    OnEnableVec.clear();
}

void ESceneManager::ObjectsOnDisable()
{
    auto& [OnDisableSet, OnDisableVec] = m_OnDisableQueue;
    for (auto& component : OnDisableVec)
    {
        component->OnDisable();
    }
    OnDisableSet.clear();
    OnDisableVec.clear();
}

void ESceneManager::ObjectsAddToLifeCycle()
{
    for (auto& gameObject : m_AddGameObjectsQueue)
    {
        int id = gameObject->m_instanceID;
        if (id < 0)
        {
            __debugbreak(); //인스턴스 아이디 오류
        }
        if (id < m_runtimeObjects.size() && m_runtimeObjects[id])
        {
            __debugbreak(); //이미 존재하는 인스턴스 아이디.
        }

        if (m_runtimeObjects.size() <= id)
        {
            m_runtimeObjects.resize(id + 1);
        }
        m_runtimeObjects[id] = gameObject;
        
    }
    m_AddGameObjectsQueue.clear();

    for (auto& component : m_AddComponentsQueue)
    {
        m_WaitAwakeVec.push_back(component);
        m_WaitStartVec.push_back(component);
    }
    m_AddComponentsQueue.clear();
}

bool ESceneManager::IsRuntimeActive(std::shared_ptr<GameObject>& obj)
{
    bool isEmpty = obj.get() == nullptr;
    bool isActive = obj->activeInHierarchy_property_getter();

    return !isEmpty && isActive;
}


