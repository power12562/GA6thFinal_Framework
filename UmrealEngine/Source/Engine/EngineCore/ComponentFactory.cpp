#include "pch.h"

EComponentFactory::EComponentFactory()
{
    SetDllDirectory(EComponentFactory::Engine::ScriptsDllPath);
}
EComponentFactory::~EComponentFactory() = default;

bool EComponentFactory::InitalizeComponentFactory()
{
    //복구해야할 컴포넌트 항목들
    static std::vector<std::tuple<GameObject*, std::string, int>> addList;
    addList.clear();

    if (m_scriptsDll != NULL)
    {
        //모든 컴포넌트 자원 회수
        for (auto& [key, wptr] : m_ComponentInstanceVec)
        {
            if (auto component = wptr.lock())
            {
                int index = component->GetComponentIndex();
                addList.emplace_back(component->m_gameObect, key, index);
                component->m_gameObect->m_components[index].reset(); //컴포넌트 파괴
            }
        }
        m_ComponentInstanceVec.clear();

        FreeLibrary(m_scriptsDll);
        m_scriptsDll = NULL;
    }

    if (!dllUtility::RunBatchFile(Engine::BuildBatchPath))
    {
        assert(!"DLL Build Fail");
        return false;
    }
      
    m_NewScriptsFunctionMap.clear();
    m_NewScriptsKeyVec.clear();
    m_scriptsDll = LoadLibraryW(L"UmrealScripts.dll");
    if (m_scriptsDll == NULL)
    {
        assert(!"DLL Loade Fail");
        return false;
    }

    std::vector<std::string> funcList = dllUtility::GetDLLFuntionNameList(m_scriptsDll);
    MakeScriptFunc = (MakeUmScriptsFile)GetProcAddress(m_scriptsDll, funcList[0].c_str());
    if (funcList[0] != "CreateUmrealcSriptFile")
    {
        FreeLibrary(m_scriptsDll);
        m_scriptsDll = NULL;
        __debugbreak(); //초기화 함수 에러.
        return false;
    }

    auto InitDLLCores = (InitScripts)GetProcAddress(m_scriptsDll, funcList[1].c_str());
    InitDLLCores(
        *EngineCore,
        ImGui::GetCurrentContext());
    if (funcList[1] != "InitalizeUmrealScript")
    {
        FreeLibrary(m_scriptsDll);
        m_scriptsDll = NULL;
        __debugbreak(); //초기화 함수 에러.
        return false;
    }

    for (size_t i = 0; i < funcList.size(); i++)
    {
        std::string& funcName = funcList[i];
        if (funcName.find("New") != std::string::npos)
        {
            auto NewComponentFunc = (NewScripts)GetProcAddress(m_scriptsDll, funcName.c_str());
            Component* component = NewComponentFunc();
            const char* key = typeid(*component).name();
            m_NewScriptsFunctionMap[key] = NewComponentFunc;
            m_NewScriptsKeyVec.emplace_back(key);
            delete component;
        }
    }

    //파괴된 컴포넌트 재생성 및 복구
    for (auto& [gameObject, key, index] : addList)
    {
        auto findIter = m_NewScriptsFunctionMap.find(key);
        if (findIter != m_NewScriptsFunctionMap.end())
        {
            //컴포넌트 존재하면 다시 생성
            std::shared_ptr<Component> newComponent = NewComponent(key);
            ResetComponent(gameObject, newComponent.get()); //엔진에서 사용하기 위해 초기화
            newComponent->m_index = index;                  //인덱스 제대로 재설정
            gameObject->m_components[index] = newComponent;
        }
    }
    //존재 안하는거는 전부 제거
    for (auto& [gameObject, key, index] : addList)
    {
        std::erase_if(gameObject->m_components, [](auto& sptr)
            {
                return sptr == nullptr;
            });
    }

    return true;
}

void EComponentFactory::UninitalizeComponentFactory()
{
    if (m_scriptsDll != NULL)
    {
        //모든 컴포넌트 자원 회수
        for (auto& [key, wptr] : m_ComponentInstanceVec)
        {
            if (auto component = wptr.lock())
            {
                int index = component->GetComponentIndex();
                component->m_gameObect->m_components[index].reset(); //컴포넌트 파괴
            }
        }
        m_ComponentInstanceVec.clear();
        FreeLibrary(m_scriptsDll);
        m_scriptsDll = NULL;
    }
}

bool EComponentFactory::AddComponentToObject(GameObject* ownerObject, std::string_view typeid_name)
{
    if(std::shared_ptr<Component> sptr_component = NewComponent(typeid_name))
    {
        const char* name = typeid_name.data();
        ResetComponent(ownerObject, sptr_component.get());
        ownerObject->m_components.emplace_back(sptr_component);  //오브젝트에 추가
        ESceneManager::Engine::AddComponentToLifeCycle(sptr_component); //씬에 등록
        return true;
    }
    return false;
}

void EComponentFactory::MakeScriptFile(const char* fileName) const
{
    MakeScriptFunc(fileName);
}

std::shared_ptr<Component> EComponentFactory::NewComponent(std::string_view typeid_name)
{
    std::shared_ptr<Component> newComponent;
    auto findIter = m_NewScriptsFunctionMap.find(typeid_name.data());
    if (findIter != m_NewScriptsFunctionMap.end())
    {
        auto& [name, func] = *findIter;
        newComponent.reset(func());                                //컴포넌트 생성
        m_ComponentInstanceVec.emplace_back(name, newComponent);   //추적용 weak_ptr 생성 
    }
    return newComponent;
}

void EComponentFactory::ResetComponent(GameObject* ownerObject, Component* component)
{
    //여긴 엔진에서 사용하기 위한 초기화 코드 
    component->m_className = (typeid(*component).name() + 5);
    component->m_gameObect = ownerObject;
    component->m_index = ownerObject->m_components.size();

    component->Reset();
    //end
}
