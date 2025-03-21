#include "pch.h"

GameObjectFactory GameObjectFactory::instance;
GameObjectFactory& gameObjectFactory = GameObjectFactory::Engine::GetInstance();

GameObjectFactory::GameObjectFactory()
{
    //엔진에 존재하는 모든 오브젝트 타입들을 등록해야합니다.
    RegisterGameObject<GameObject>();

}

GameObjectFactory::~GameObjectFactory() = default;

std::shared_ptr<GameObject> GameObjectFactory::NewGameObject(std::string_view typeid_name)
{
    std::shared_ptr<GameObject> newObject;
    auto findIter = m_NewGameObjectFuncMap.find(typeid_name.data());
    if (findIter != m_NewGameObjectFuncMap.end())
    {
        auto& [key, NewObjectFunc] = *findIter;
        newObject.reset(NewObjectFunc());
    }
    return newObject;
}

void GameObjectFactory::ResetGameObject(GameObject* ownerObject)
{
    
}
