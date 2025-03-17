#include "pch.h"

ComponentFactory ComponentFactory::instance;
ComponentFactory& componentFactory = ComponentFactory::instance;

ComponentFactory::ComponentFactory() = default;
ComponentFactory::~ComponentFactory() = default;

bool ComponentFactory::InitalizeComponentFactory()
{
    //복구해야할 컴포넌트 항목들
    static std::vector<std::tuple<GameObject*, std::string, int>> addList;
    addList.clear();

    if (m_scriptsDll != NULL)
    {
        //모든 컴포넌트 자원 회수
        for (auto& [key, vec] : m_ComponentInstanceMap)
        {
            for (auto& wptr : vec)
            {
                if (auto component = wptr.lock())
                {
                    int index = component->GetComponentIndex();
                    addList.emplace_back(component->m_gameObect, key, index);
                    component->m_gameObect->m_components[index].reset(); //컴포넌트 파괴
                }
            }
            vec.clear();
        }

        FreeLibrary(m_scriptsDll);
        m_scriptsDll = NULL;
    }

    if (!Utility::RunBatchFile(BuildBatchPath))
        return false;

    m_scriptsDll = LoadLibraryW(ScriptsPath);
    if (m_scriptsDll != NULL)
    {
        std::vector<std::string> funcList = Utility::GetDLLFuntionNameList(m_scriptsDll);
        if (funcList[0] != "InitalizeUmrealScript")
        {
            FreeLibrary(m_scriptsDll);
            m_scriptsDll = NULL;
            __debugbreak(); //스크립트 초기화 함수 에러.
            return false;
        }

        auto InitDLLCores = (InitScripts)GetProcAddress(m_scriptsDll, funcList[0].c_str());
        InitDLLCores(EngineCores{
            Time,
            sceneManager,
            componentFactory
            });

        m_NewScriptsFunctionMap.clear();
        m_NewScriptsFunctionVec.clear();
        for (size_t i = 1; i < funcList.size(); i++)
        {
            std::string& funcName = funcList[i];
            if (funcName.find("New") != std::string::npos)
            {
                auto NewComponentFunc = (NewScripts)GetProcAddress(m_scriptsDll, funcName.c_str());
                Component* component = NewComponentFunc();
                const char* key = typeid(*component).name();
                m_NewScriptsFunctionMap[key] = m_NewScriptsFunctionVec.size();
                m_NewScriptsFunctionVec.emplace_back(key, NewComponentFunc);
                delete component;
            }
        }
        
        //파괴된 컴포넌트 복구 추가해야함
        for (auto& [gameObject, key, index] : addList)
        {
           
        }
    }
}

void ComponentFactory::UninitalizeComponentFactory()
{
    if (m_scriptsDll != NULL)
    {
        FreeLibrary(m_scriptsDll);
        m_scriptsDll = NULL;
    }
}

bool ComponentFactory::NewComponent(GameObject* ownerObject, std::string_view typeid_name)
{
    Component* newComponent = nullptr;
    auto findIter = m_NewScriptsFunctionMap.find(typeid_name.data());
    if (findIter != m_NewScriptsFunctionMap.end())
    {
        auto& [name, index] = *findIter;
        newComponent = m_NewScriptsFunctionVec[index].second();    //컴포넌트 생성
        std::shared_ptr<Component> sptr_component{newComponent};   //shared_ptr 생성
        m_ComponentInstanceMap[name].emplace_back(sptr_component); //추적용 weak_ptr 생성 
        ResetComponent(ownerObject, newComponent);
        ownerObject->m_components.emplace_back(sptr_component);  //오브젝트에 추가
        return true;
    }
    return false;
}

void ComponentFactory::ResetComponent(GameObject* ownerObject, Component* component)
{
    //여긴 엔진에서 사용하기 위한 초기화 코드 
    component->m_gameObect = ownerObject;
    component->m_index = ownerObject->m_components.size();

    component->Reset();
    //end
}
