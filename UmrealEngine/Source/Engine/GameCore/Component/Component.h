#pragma once

//참고 Unity Game Loop https://docs.unity3d.com/kr/2022.3/Manual/ExecutionOrder.html
class Component
{
    friend class GameObject;
    friend class ComponentFactory;
public:
    Component();
    virtual ~Component();

    /// <summary>
    /// 이 함수는 항상 Start 함수 전에 호출되며 프리팹이 인스턴스화 된 직후에 호출됩니다.
    /// 게임 오브젝트의 Active가 false 상태인 경우 Awake 함수는 true가 될때까지 호출되지 않습니다.
    /// </summary>
    virtual void Awake() {};

    /// <summary>
    /// 오브젝트가 활성화된 경우에만 호출됩니다.
    /// 컴포넌트의 Enable 활성화 직후 이 함수를 호출합니다 false 상태인 경우 true가 될때까지 호출되지 않습니다.
    /// </summary>
    virtual void OnEnable() {};

    /// <summary>
    /// FixedUpdate 는 종종 Update 보다 더 자주 호출됩니다. 
    /// 프레임 속도가 낮은 경우 프레임당 여러 번 호출될 수 있으며 프레임 속도가 높은 경우 프레임 사이에 호출되지 않을 수 있습니다. 
    /// </summary>
    virtual void FixedUpdate() {};

    /// <summary>
    /// Update 는 프레임당 한 번 호출됩니다. 
    /// </summary>
    virtual void Update() {};

    /// <summary>
    /// LateUpdate 는 Update가 모두 끝난 후 호출됩니다.
    /// </summary>
    virtual void LateUpdate() {};
  

public:
    REFLECT_FIELDS_BASE_BEGIN()
    REFLECT_FIELDS_BASE_END(Component, public)
};