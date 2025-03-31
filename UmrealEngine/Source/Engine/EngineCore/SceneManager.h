#pragma once
class GameObject;
class ESceneManager;

//참고 
// Unity SceneManager https://docs.unity3d.com/6000.0/Documentation/ScriptReference/SceneManagement.SceneManager.html
// Unity Scene https://docs.unity3d.com/6000.0/Documentation/ScriptReference/SceneManagement.Scene.html

//이 구조체는 단순히 씬의 정보를 나열한 구조체에 불과함.
//일단 선언만. 구현은 나중에.
struct Scene
{
public:
    USING_PROPERTY(Scene)
    GETTER_ONLY(size_t, buildIndex)
    {
        return 0;
    }
    //구현 x
    //get : 빌드 설정에서의 Scene 인덱스를 반환합니다.
    PROPERTY(buildIndex);

    GETTER_ONLY(bool, isDirty)
    {
        return false;
    }
    //구현 x
    //get : 씬이 수정된 경우 true를 반환합니다.
    PROPERTY(isDirty);

    GETTER_ONLY(bool, isLoaded)
    {
        return false;
    }
    //구현 x
    //get : 이 씬의 로드 여부를 반환합니다.
    PROPERTY(isLoaded);

    GETTER_ONLY(std::wstring_view, name)
    {
        return L"";
    }
    //구현 x
    //get : 이 씬의 파일 이름을 반환합니다.
    PROPERTY(name);

    GETTER_ONLY(std::wstring_view, path)
    {
        return L"";
    }
    //구현 x
    //get : 이 씬 파일의 상대 경로를 반환합니다.
    PROPERTY(path);

    GETTER_ONLY(size_t, rootCount)
    {
        return 0;
    }
    //구현 x
    //get : 이 씬의 Root GameObject 개수를 반환합니다. (자식 오브젝트는 포함하지 않습니다.)
    PROPERTY(rootCount);

    /// <summary>
    /// <para> 구현 X                                                                       </para>
    /// <para> 씬에 속한 Root GameObject들을 반환합니다. (자식 오브젝트는 포함하지 않습니다.)   </para>
    /// </summary>
    /// <returns></returns>
    std::vector<std::weak_ptr<GameObject>> GetRootGameObjects() { return std::vector<std::weak_ptr<GameObject>>(); }

    /// <summary>
    /// <para>  구현 X                    </para>
    /// <para>  씬이 유효한지 확인합니다.  </para>
    /// </summary>
    /// <returns></returns>
    bool IsValid() { return false; }

    /// <summary>
    /// 다른 Scene인지 경로를 비교합니다.
     /// </summary>
    /// <returns>비교 결과</returns>
    bool operator!=(const Scene& other)
    {
        return (std::wstring_view)path != other.path;
    }
    /// <summary>
    /// 같은 Scene인지 경로를 비교합니다.
    /// </summary>
    /// <returns>비교 결과</returns>
    bool operator==(const Scene& other)
    {
        return (std::wstring_view)path == other.path;
    }
};

//함수는 일단 선언만. 구현은 나중에.
class ESceneManager
{
    friend class EngineCores;
public:
    //엔진 접근용 네임스페이스
    struct Engine
    {
        /// <summary>
        /// 씬 매니저가 관리하는 오브젝트를 전부 정리합니다.
        /// </summary>
        static void CleanupSceneManager();

        /// <summary>
        /// 씬 매니저가 관리하는 오브젝트들을 업데이트합니다. 클라이언트에서 매 틱 호출해야합니다.
        /// </summary>
        static void SceneUpdate();

        /// <summary>
        /// 씬에 게임 오브젝트를 추가합니다.
        /// </summary>
        /// <param name="GameObject :">추가할 오브젝트</param>
        static void AddGameObjectToLifeCycle(std::shared_ptr<GameObject> gameObject);

        /// <summary>
        /// 컴포넌트를 Awake, Start를 호출하기 위한 대기열에 추가합니다.
        /// </summary>
        /// <param name="component"></param>
        static void AddComponentToLifeCycle(std::shared_ptr<Component> component);

        /// <summary>
        /// <para> 게임 오브젝트의 라이프 사이클 활성화 여부를 변경합니다. </para>
        /// <para> false로 설정시 라이프 사이클 함수를 수행하지 않습니다. </para>
        /// <param name="instanceID :">적용 대상의 instanceID</param>
        /// <param name="value :">적용 값</param>
        /// </summary>
        static void SetGameObjectActive(int instanceID, bool value);

        /// <summary>
        /// <para> 컴포넌트의 라이프 사이클 활성화 여부를 변경합니다.     </para>
        /// <para> false로 설정시 라이프 사이클 함수를 수행하지 않습니다. </para>
        /// </summary>
        /// <param name="component :">적용 대상</param>
        /// <param name="value :">적용 값</param>
        static void SetComponentEnable(Component* component, bool value);

        /// <summary>
        /// 게임 오브젝트의 이름으로 오브젝트를 탐색합니다.
        /// </summary>
        /// <param name="name :">찾을 오브젝트의 이름</param>
        /// <returns>성공시 weak_ptr에 담아줍니다.</returns>
        static std::weak_ptr<GameObject> FindGameObjectWithName(std::wstring_view name);

        /// <summary>
        /// 게임 오브젝트의 이름을 변경합니다.
        /// </summary>
        /// <param name="gameObject :">대상</param>
        /// <param name="newName :">새로운 이름</param>
        static void RenameGameObject(GameObject* gameObject, std::wstring_view newName);

        /// <summary>
        /// m_runtimeRootObjects의 항목을 갱신합니다. Transform의 부모 자식 관계가 변경될때마다 호출됩니다.
        /// </summary>
        static void UpdateRootObjects();
    };

private:
    ESceneManager();
    ~ESceneManager();

public:
    //현재 라이프 사이클에 포함된 루트 오브젝트를 전부 반환합니다.
    const std::vector<std::weak_ptr<GameObject>>& GetRootObjects() const
    {
        return m_runtimeRootObjects;
    }

private:
    //Life cycle을 수행. 클라에서 매틱 호출해야함.
    void SceneUpdate();

private:
    void ObjectsAwake();            //Awake 예정인 컴포넌트들의 Awake 함수를 호출합니다.
    void ObjectsOnEnable();         //OnEnable 예정인 컴포넌트들의 OnEnable 함수를 호출합니다.
    void ObjectsStart();            //Start 예정인 컴포넌트들의 Start 함수를 호출합니다.
    void ObjectsFixedUpdate();      //FixedUpdate를 호출합니다.
    void ObjectsUpdate();           //Update를 호출합니다.
    void ObjectsLateUpdate();       //LateUpdate를 호출합니다.
    void ObjectsOnDisable();        //OnDisable 예정인 컴포넌트들의 OnDisable 함수를 호출해줍니다.
    void ObjectsAddToLifeCycle();   //추가 대기중인 오브젝트, 컴포넌트를 라이프 사이클에 포함시킵니다.
private:
    /*게임오브젝트의 Life cycle 수행 여부를 확인하는 함수*/
    bool IsRuntimeActive(std::shared_ptr<GameObject>& obj);

private:
    //Life cycle에 포함되는 실제 오브젝트들 항목
    std::vector<std::shared_ptr<GameObject>> m_runtimeObjects;

    //모든 루트 오브젝트 캐싱용
    std::vector<std::weak_ptr<GameObject>> m_runtimeRootObjects;

    //오브젝트 이름과 포인터로 관리하는 map
    std::unordered_map<std::wstring, std::unordered_set<std::shared_ptr<GameObject>>> m_runtimeObjectsUnorderedMap;

    //오브젝트 추가 대기열 
    std::vector<std::shared_ptr<GameObject>> m_AddGameObjectsQueue;
    std::vector<std::shared_ptr<Component>> m_AddComponentsQueue;

    //초기화 함수 호출 대기열
    std::vector<std::shared_ptr<Component>> m_WaitAwakeVec;
    std::vector<std::shared_ptr<Component>> m_WaitStartVec;

    //OnEnable, OnDisable을 set과 같이 관리
    std::tuple<std::unordered_set<Component*>, std::vector<Component*>, std::vector<bool*>> m_OnEnableQueue;
    std::tuple<std::unordered_set<Component*>, std::vector<Component*>, std::vector<bool*>> m_OnDisableQueue;
};