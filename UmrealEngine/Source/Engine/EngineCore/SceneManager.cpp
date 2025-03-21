#include "pch.h"

SceneManager SceneManager::instance;
SceneManager& sceneManager = SceneManager::Engine::GetInstance();

void SceneManager::Engine::SceneUpdate()
{
    sceneManager.SceneUpdate();
}

void SceneManager::SceneUpdate()
{
    TimeSystem::Engine::TimeSystemUpdate();
    while (TimeSystem::Engine::TimeSystemFixedUpdate())
    {
        ObjectsFixedUpdate();
    }
    ObjectsUpdate();
    ObjectsLateUpdate();
}

void SceneManager::ObjectsFixedUpdate()
{
    for (auto& obj : m_runtimeObjects)
    {
        if (IsRuntimeActive(obj))
        {
            for (auto& component : obj->m_components)
            {
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
                component->LateUpdate();
            }
        }
    }
}

bool SceneManager::IsRuntimeActive(std::shared_ptr<GameObject>& obj)
{
    bool isEmpty = obj.get() == nullptr;
    bool isActive = obj->activeInHierarchy_property_getter();

    return !isEmpty && isActive;
}


