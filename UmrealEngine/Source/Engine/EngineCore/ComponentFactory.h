#pragma once
class GameObject;
class Component;
class EngineCores;
class ComponentFactory;
#ifndef SCRIPTS_PROJECT
extern ComponentFactory& componentFactory;
#endif

class ComponentFactory
{
public:
    static ComponentFactory instance;

    //dll 경로
#ifdef _DEBUG
    static constexpr const wchar_t* ScriptsPath = L"../UmrealScripts/bin/Debug/UmrealScripts.dll";
    static constexpr const wchar_t* BuildBatchPath = L"..\\UmrealScripts\\build_debug.bat";
#else
    static constexpr const wchar_t* ScriptsPath = L"../UmrealScripts/bin/Release/UmrealScripts.dll";
    static constexpr const wchar_t* BuildBatchPath = L"..\\UmrealScripts\\build_release.bat";
#endif
private:
    ComponentFactory();
    ~ComponentFactory();

public:
    /// <summary>
    /// <summary>컴포넌트 팩토리를 초기화합니다. </summary>
    /// DLL 빌드 -> DLL 로드 -> 팩토리 초기화 순으로 진행됩니다.
    /// 이미 초기화되어있으면 DLL언로드 후 다시 로드합니다.
    /// 생성되어있던 모든 컴포넌트들도 파괴 후 다시 생성합니다.
    /// </summary>
    /// <returns>성공 여부</returns>
    bool InitalizeComponentFactory();

    /// <summary>
    /// 스크립트 DLL을 언로드합니다.
    /// </summary>
    void UninitalizeComponentFactory();

    /// <summary>
    /// 컴포넌트 클래스 typeid로 컴포넌트를 생성합니다.
    /// </summary>
    /// <param name="ownerObject :">컴포넌트를 추가할 오브젝트</param>
    /// <param name="typeid_name :">컴포넌트 typeid().name()</param>
    /// <returns>성공 여부</returns>
    bool NewComponent(GameObject* ownerObject, std::string_view typeid_name);

    /// <summary>
    /// 스크립트 DLL의 모든 컴포넌트 생성자들을 반환합니다.
    /// </summary>
    /// <returns></returns>
    const std::vector<std::pair<std::string, Component*(*)()>>& GetNewComponentFuncList()
    {
        return m_NewScriptsFunctionVec;
    }
private:
    using InitScripts = void(*)(const EngineCores& core);
    using NewScripts = Component*(*)();

    HMODULE m_scriptsDll{};
    std::map<std::string, size_t> m_NewScriptsFunctionMap{};
    std::vector<std::pair<std::string, NewScripts>> m_NewScriptsFunctionVec{}; 

    std::unordered_map<std::string, std::vector<std::weak_ptr<Component>>> m_ComponentInstanceMap;

private:
    //컴포넌트를 엔진에 사용하기 위해 초기화합니다.
    void ResetComponent(GameObject* ownerObject, Component* component);
};