#pragma once
class GameObject;
class SceneManager;
#ifndef UMREALSCRIPTS_EXPORT
extern SceneManager& sceneManager;
#endif

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
};

//함수는 일단 선언만. 구현은 나중에.
class SceneManager
{
public:
    //싱글톤용
    static SceneManager instance;
private:
    SceneManager() = default;
    ~SceneManager() = default;
public:
};