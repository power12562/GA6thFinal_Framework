#pragma once

//씬그래프, position, rotation, scale을 제어하는 Transform 코어클래스
class Transform
{
public:
    Transform(GameObject& owner);
    ~Transform();
    GameObject& gameObject;

public:
    /// <summary>
    /// <para> https://docs.unity3d.com/6000.0/Documentation/ScriptReference/Transform.DetachChildren.html </para>
    /// <para> 자식오브젝트들을 전부 분리해 root로 만들어버립니다.                                             </para>
    /// </summary>
    void DetachChildren();

    /// <summary>
    /// <para> https://docs.unity3d.com/6000.0/Documentation/ScriptReference/Transform.SetParent.html  </para>
    /// <para> Transform의 부모를 설정합니다. nullptr 전달시 이 Transform을 root로 만듭니다.               </para>
    /// </summary>
    /// <param name="p :">부모로 설정할 대상</param>
    void SetParent(Transform* p);
    void SetParent(Transform& p);

    /// <summary>
    /// <para> https://docs.unity3d.com/6000.0/Documentation/ScriptReference/Transform.GetChild.html </para>
    /// <para> 인덱스로 자식을 가져옵니다.                                                             </para>
    /// </summary>
    /// <param name="index :">자식의 인덱스</param>
    /// <returns>성공시 해당 자식의 포인터. 실패시 nullptr</returns>
    Transform* GetChild(int index) const
    {
        Transform* child = nullptr;
        if (index < m_childsList.size())
        {
            child = m_childsList[index];
        }
        return child;
    }

    /// <summary>
    /// https://docs.unity3d.com/6000.0/Documentation/ScriptReference/Transform.Find.html
    /// <para> 이름으로 child를 찾아 반환합니다.                                                                                               </para>
    /// <para> 이름이 있는 자식을 찾을 수 없으면 null이 반환됩니다.'/' 문자가 포함된 경우 경로 이름처럼 계층 구조에서 Transform에 액세스합니다.      </para>
    /// <para> 참고: GameObject 이름에 '/'가 있는 경우 Find가 제대로 작동하지 않습니다.                                                          </para>
    /// <para> 참고 : Find는 Transform 계층 구조에서 재귀적으로 내려가지 않습니다.                                                               </para>
    /// <para> 참고 : Find는 비활성화된 GameObject의 Transform을 찾을 수 있습니다.                                                              </para>
    /// <para> 참고 : 동일한 이름이 존재하면 첫번째 오브젝트를 반환합니다.                                                                        </para>
    /// </summary>
    /// <param name="name :">찾을 오브젝트 이름</param>
    /// <returns></returns>
    Transform* Find(std::wstring_view name) const;

private:
    Transform* m_root = nullptr;
    Transform* m_parent = nullptr;
    std::vector<Transform*> m_childsList;

private:
    /// <summary>
    /// 부모를 지웁니다.
    /// </summary>
    void EraseParent();

    /// <summary>
    /// 대상의 모든 자식을 순회하면서 root를 변경합니다.
    /// </summary>
    /// <param name="target :">루트</param>
    void SetChildsRootParent(Transform* root);

    /// <summary>
    /// 모든 부모를 확인하면서 전달받은 Transform이 존재하는지 확인합니다.
    /// </summary>
    /// <param name="potentialAncestor :">부모로 설정할 대상</param>
    /// <returns>존재 여부</returns>
    bool IsDescendantOf(Transform* potentialAncestor) const;

public:
    USING_PROPERTY(Transform);
    GETTER_ONLY(int, childCount)
    {
        return m_childsList.size();
    }
    // https://docs.unity3d.com/6000.0/Documentation/ScriptReference/Transform-childCount.html
    //get : 자식의 개수를 반환합니다. 
    //return : int
    PROPERTY(childCount);

    GETTER_ONLY(Transform*, root)
    {
        return m_root;
    }
    // https://docs.unity3d.com/6000.0/Documentation/ScriptReference/Transform-root.html
    //get : 최상위 부모를 반환합니다.
    //return : Transform*
    PROPERTY(root);

    GETTER_ONLY(Transform*, parent)
    {
        return m_parent;
    }
    // https://docs.unity3d.com/6000.0/Documentation/ScriptReference/Transform-parent.html
    //get : 부모를 반환합니다.
    //return : Transform*
    PROPERTY(parent);
};

