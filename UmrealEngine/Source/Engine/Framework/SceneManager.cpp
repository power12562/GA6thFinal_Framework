#include "pch.h"
#include "SceneManager.h"

SceneManager SceneManager::_SceneManager;
SceneManager& _SceneManager = SceneManager::_SceneManager;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::ClientUpdate()
{
    while (TimeSystem::Engine::TimeSystemFixedUpdate())
    {
        FixedUpdate();
    }
    Update();
}

void SceneManager::ClientRender()
{
    Render();
}

void SceneManager::UpdateScene()
{
    if (mNextScene)
    {
        if (mCurrScene)
        {
            // mCurrScene::Exit
        }
        mCurrScene = mNextScene;
        mNextScene = nullptr;
        // mCurrScene::Enter
    }
}

void SceneManager::FixedUpdate()
{
}

void SceneManager::Update()
{
}

void SceneManager::Render()
{
}
