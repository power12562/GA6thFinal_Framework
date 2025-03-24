#include "pch.h"

void ESceneManager::Engine::SceneUpdate()
{
    EngineCore->SceneManager.SceneUpdate();
}

void ESceneManager::Engine::AddGameObjectToLifeCycle(std::shared_ptr<GameObject> gameObject)
{
    auto [iter, result] = EngineCore->SceneManager.m_runtimeObjectsUnorderedMap[gameObject->m_name].insert(gameObject);
    if (result == false)
    {
        assert(!"이미 추가한 게임 오브젝트 입니다.");
    }
    else
    {
        EngineCore->SceneManager.m_AddGameObjectsQueue.push_back(gameObject);
    }
}

void ESceneManager::Engine::AddComponentToLifeCycle(std::shared_ptr<Component> component)
{
    EngineCore->SceneManager.m_AddComponentsQueue.push_back(component);
}

void ESceneManager::Engine::SetGameObjectActive(int instanceID, bool value)
{
    if (auto& gameObject = EngineCore->SceneManager.m_runtimeObjects[instanceID])
    {
        if (gameObject->m_activeSelf != value)
        {
            //컴포넌트들의 On__able 함수를 호출하도록 합니다.
            auto& [WaitSet, WaitVec, WaitValue] = value ? EngineCore->SceneManager.m_OnEnableQueue : EngineCore->SceneManager.m_OnDisableQueue;
            WaitValue.emplace_back(&gameObject->m_activeSelf);
            for (auto& component : gameObject->m_components)
            {
                if (component->m_enable)
                {
                    auto [iter, result] = WaitSet.insert(component.get());
                    if (result)
                    {
                        WaitVec.emplace_back(component.get());
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
        //컴포넌트의 On__able 함수를 호출하도록 합니다.
        auto& [WaitSet, WaitVec, WaitValue] = value ? EngineCore->SceneManager.m_OnEnableQueue : EngineCore->SceneManager.m_OnDisableQueue;
        WaitValue.emplace_back(&component->m_enable);
        if (component->gameObect->m_activeSelf)
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
    auto findIter = EngineCore->SceneManager.m_runtimeObjectsUnorderedMap.find(name.data());
    if (findIter != EngineCore->SceneManager.m_runtimeObjectsUnorderedMap.end() && !findIter->second.empty())
    {
        findObject = *findIter->second.begin();
    }
    return findObject;
}

void ESceneManager::Engine::RenameGameObject(GameObject* gameObject, std::wstring_view newName)
{
    if (gameObject == nullptr)
        return;

    if (gameObject->m_name != newName)
    {
        auto& ObjectsNameMap = EngineCore->SceneManager.m_runtimeObjectsUnorderedMap;
        auto mapIter = ObjectsNameMap.find(gameObject->m_name);
        if (mapIter != ObjectsNameMap.end())
        {
            const std::shared_ptr<GameObject>* sptr = nullptr;
            auto& [name, set] = *mapIter;
            for (auto& obj : set)
            {
                if (obj.get() == gameObject)
                {
                    sptr = &obj;
                    break;
                }
            }
            if (sptr)
            {
                ObjectsNameMap[newName.data()].insert(*sptr);
                set.erase(*sptr);
                gameObject->m_name = newName;
                if (set.empty())
                {
                    ObjectsNameMap.erase(mapIter);
                }
                return;
            }
        }
    }
    __debugbreak(); //이름 오류.
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
    auto& [OnEnableSet, OnEnableVec, OnEnableValue] = m_OnEnableQueue;
    for (auto& value : OnEnableValue)
    {
        *value = true;  
    }
    for (auto& component : OnEnableVec)
    {
        component->OnEnable();
    }
    OnEnableSet.clear();
    OnEnableVec.clear();
    OnEnableValue.clear();
}

void ESceneManager::ObjectsOnDisable()
{
    auto& [OnDisableSet, OnDisableVec, OnDisableValue] = m_OnDisableQueue;
    for (auto& value : OnDisableValue)
    {
        *value = false;
    }
    for (auto& component : OnDisableVec)
    {
        component->OnDisable();
    }
    OnDisableSet.clear();
    OnDisableVec.clear();
    OnDisableValue.clear();
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
        m_runtimeRootObjects.emplace_back(gameObject);
        std::erase_if(m_runtimeRootObjects, [](const std::weak_ptr<GameObject>& weak)
            {
                if (weak.expired())
                    return true;    //파괴된 오브젝트는 전부 제거

                if (std::shared_ptr<GameObject> shared = weak.lock())
                {
                    //shared->transform->isParent; //최상위 부모만 남겨야함
                }

                return false;
            });
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


