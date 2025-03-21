#include "pch.h"

SceneManager SceneManager::instance;
SceneManager& sceneManager = SceneManager::Engine::GetInstance();

void SceneManager::Engine::SceneUpdate()
{
    sceneManager.SceneUpdate();
}

void SceneManager::Engine::AddGameObjectToLifeCycle(std::shared_ptr<GameObject> gameObject)
{
    auto [iter, result] = sceneManager.m_runtimeObjectsUnorderedMap[gameObject->m_name].insert(gameObject);
    if (result == false)
    {
        assert(!"이미 추가한 게임 오브젝트 입니다.");
    }
    else
    {
        sceneManager.m_AddGameObjectsQueue.push_back(gameObject);
    }
}

void SceneManager::Engine::AddComponentToLifeCycle(std::shared_ptr<Component> component)
{
    sceneManager.m_AddComponentsQueue.push_back(component);
}

void SceneManager::Engine::SetGameObjectActive(int instanceID, bool value)
{
    if (auto& gameObject = sceneManager.m_runtimeObjects[instanceID])
    {
        if (gameObject->m_activeSelf != value)
        {
            gameObject->m_activeSelf = value;
            //컴포넌트들의 On__able 함수를 호출하도록 합니다.
            for (auto& component : gameObject->m_components)
            {
                std::unordered_set<Component*>& WaitSet = value ? sceneManager.m_WaitOnEnableSet : sceneManager.m_WaitOnDisableSet;
                std::vector<Component*>& WaitVec = value ? sceneManager.m_WaitOnEnableVec : sceneManager.m_WaitOnDisableVec;
                auto [iter, result] = WaitSet.insert(component.get());
                if (result)
                {
                    WaitVec.push_back(component.get()); 
                }
            }
        }
    }
}

void SceneManager::Engine::SetComponentEnable(Component* component, bool value)
{
    if (component && component->m_enable != value)
    {
        component->m_enable = value;
        //컴포넌트의 On__able 함수를 호출하도록 합니다.
        std::unordered_set<Component*>& WaitSet = value ? sceneManager.m_WaitOnEnableSet : sceneManager.m_WaitOnDisableSet;
        std::vector<Component*>& WaitVec = value ? sceneManager.m_WaitOnEnableVec : sceneManager.m_WaitOnDisableVec;
        auto [iter, result] = WaitSet.insert(component);
        if (result)
        {
            WaitVec.push_back(component);
        }
    }
}

std::weak_ptr<GameObject> SceneManager::Engine::FindGameObjectWithName(std::wstring_view name)
{
    std::weak_ptr<GameObject> findObject;
    auto findIter = sceneManager.m_runtimeObjectsUnorderedMap.find(name.data());
    if (findIter != sceneManager.m_runtimeObjectsUnorderedMap.end() && !findIter->second.empty())
    {
        findObject = *findIter->second.begin();
    }
    return findObject;
}

void SceneManager::SceneUpdate()
{
    TimeSystem::Engine::TimeSystemUpdate();
    ObjectsAwake();
    ObjectsOnEnable();
    ObjectsStart();
    while (TimeSystem::Engine::TimeSystemFixedUpdate())
    {
        ObjectsFixedUpdate();
    }
    ObjectsUpdate();
    ObjectsLateUpdate();
    ObjectsOnDisable();
    ObjectsAddToLifeCycle();
}

void SceneManager::ObjectsAwake()
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

void SceneManager::ObjectsOnEnable()
{
    for (auto& component : m_WaitOnEnableVec)
    {
        component->m_enable = true;
        component->OnEnable();
    }
    m_WaitOnEnableSet.clear();
    m_WaitOnEnableVec.clear();
}

void SceneManager::ObjectsStart()
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

void SceneManager::ObjectsFixedUpdate()
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

void SceneManager::ObjectsUpdate()
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

void SceneManager::ObjectsLateUpdate()
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

void SceneManager::ObjectsOnDisable()
{
    for (auto& component : m_WaitOnDisableVec)
    {
        component->m_enable = false;
        component->OnDisable();
    }
    m_WaitOnDisableSet.clear();
    m_WaitOnDisableVec.clear();
}

void SceneManager::ObjectsAddToLifeCycle()
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

bool SceneManager::IsRuntimeActive(std::shared_ptr<GameObject>& obj)
{
    bool isEmpty = obj.get() == nullptr;
    bool isActive = obj->activeInHierarchy_property_getter();

    return !isEmpty && isActive;
}


