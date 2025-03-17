#pragma once
class Component;

//참고
//Unity GameObject https://docs.unity3d.com/kr/2021.1/Manual/class-GameObject.html
//Unity GameObject Script https://docs.unity3d.com/6000.0/Documentation/ScriptReference/GameObject.html

//함수는 일단 선언만. 구현은 나중에. 
class GameObject final
{
    friend class ComponentFactory;

    //public static 함수
public:
    /// <summary>
    /// <para> 구현 X </para>
    /// <para> 매개변수와 같은 이름을 가진 GameObject를 찾아 반환합니다. </para>
    /// <para> 같은 이름의 GameObject가 없으면 nullptr를 반환합니다.    </para>
    /// <para> 참고 : 같은 이름의 오브젝트가 여러개 있으면 특정 오브젝트 반환을 보장하지 못합니다.  </para>
    /// </summary>
    /// <param name="name :">검색할 오브젝트의 이름</param>
    /// <returns>찾은 오브젝트를 weak_ptr에 담아준다.</returns>
    static std::weak_ptr<GameObject> Find(std::wstring_view name) { return std::weak_ptr<GameObject>(); }

    /// <summary>
    /// <para> 구현 X                                                      </para>
    /// <para> 매개변수와 같은 태그가 설정된 GameObject들의 배열을 반환합니다. </para>
    /// <para> 태그가 있는 GameObject가 없으면 빈 배열을 반환합니다.          </para>
    /// </summary>
    /// <param name="tag :">검색할 태그</param>
    /// <returns>찾은 오브젝트들을 담은 weak_ptr배열</returns>
    static std::vector<std::weak_ptr<GameObject>> FindGameObjectsWithTag(std::wstring_view tag) { return std::vector<std::weak_ptr<GameObject>>(); }

    /// <summary>
    /// <para> 구현 X                                                                               </para>
    /// <para> 매개변수와 같은 태그가 설정된 GameObject를 찾아 반환합니다.                             </para>
    /// <para> 참고 : 같은 태그가 설정된 오브젝트가 여러개 있으면 특정 오브젝트 반환을 보장하지 못합니다. </para>
    /// </summary>
    /// <param name="tag :">검색할 태그</param>
    /// <returns>찾은 오브젝트를 weak_ptr에 담아준다.</returns>
    static std::weak_ptr<GameObject> FindWithTag(std::wstring_view tag) { return std::weak_ptr<GameObject>(); }
public:
    GameObject();
    ~GameObject();

private:
    std::vector<std::shared_ptr<Component>>  m_components;
};