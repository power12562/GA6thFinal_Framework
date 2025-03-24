#include "pch.h"

EGameObjectFactory::EGameObjectFactory()
{
    //엔진에 존재하는 모든 오브젝트 타입들을 등록해야합니다.
    RegisterGameObject<GameObject>();

}

EGameObjectFactory::~EGameObjectFactory() = default;

std::shared_ptr<GameObject> EGameObjectFactory::NewGameObject(std::string_view typeid_name, std::wstring_view name)
{
    auto sptr_object = MakeGameObject(typeid_name);
    ResetGameObject(sptr_object.get(), name);
    ESceneManager::Engine::AddGameObjectToLifeCycle(sptr_object);
    return sptr_object;
}

std::shared_ptr<GameObject> EGameObjectFactory::MakeGameObject(std::string_view typeid_name)
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

void EGameObjectFactory::ResetGameObject(
    GameObject* ownerObject, 
    std::wstring_view name)
{
    ownerObject->m_name = name;
    ownerObject->m_isStatic = false;
    ownerObject->m_activeSelf = true;
  
    //인스턴스 아이디 부여
    int instanceID = -1;
    if (InstanceIDManager.emptyID.empty())
    {
        instanceID = InstanceIDManager.backID++;
    }
    else
    {
        instanceID = InstanceIDManager.emptyID.front();
        InstanceIDManager.emptyID.pop();
    }
    ownerObject->m_instanceID = instanceID;
}
