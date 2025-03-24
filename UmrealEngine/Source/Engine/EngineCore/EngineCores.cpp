#include "pch.h"

std::unique_ptr<EngineCores> EngineCore
{
    new EngineCores
    {
        ETimeSystem::Engine::GetInstance(),
        ESceneManager::Engine::GetInstance(),
        EComponentFactory::Engine::GetInstance(),
        EGameObjectFactory::Engine::GetInstance()
    }
};

