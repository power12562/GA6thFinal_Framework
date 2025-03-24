#include "pch.h"

Transform::Transform(GameObject& owner)
    :
    gameObject(owner)
{

}
Transform::~Transform()
{
    EraseParent();
    std::vector<Transform*> transformStack;
    for (auto& transform : m_childsList)
    {
        transformStack.push_back(transform);
    }
    this->DetachChildren();
    while (!m_childsList.empty())
    {
        Transform* currTr = transformStack.back();
        transformStack.pop_back();
        for (auto& transform : currTr->m_childsList)
        {
            transformStack.push_back(transform);
        }
        currTr->DetachChildren();
    }
}

void Transform::DetachChildren()
{
    for (auto& child : m_childsList)
    {
        child->SetParent(nullptr);
    }
}

void Transform::SetParent(Transform* p)
{
    if (p == nullptr)
    {
        EraseParent();
    }
    else //부모 관계 변경
    {
        if (p == this || p == m_parent || IsDescendantOf(this))
        {
            return;
        }
        EraseParent();
        {
            m_parent = p;

            if (p->m_root)
                m_root = p->m_root;
            else
                m_root = m_parent;

            p->m_childsList.push_back(this);
            SetChildsRootParent(m_root);
        }
    }
    ESceneManager::Engine::UpdateRootObjects();
}

void Transform::SetParent(Transform& p)
{
    SetParent(&p);
}


void Transform::EraseParent()
{
    bool isParent = this->m_parent != nullptr;
    if (isParent)
    {
        if (!m_parent->m_childsList.empty())
        {
            std::erase(m_parent->m_childsList, this); //부모의 자식 항목에 자신을 제거
        }
        m_root = nullptr;
        m_parent = nullptr;
        SetChildsRootParent(this);
    }
}

bool Transform::IsDescendantOf(Transform* potentialAncestor) const
{
    Transform* currentParent = m_parent;
    while (currentParent)
    {
        if (currentParent == potentialAncestor)
            return true;
        currentParent = currentParent->m_parent;
    }
    return false;
}

void Transform::SetChildsRootParent(Transform* _root)
{
    std::vector<Transform*> transformStack;
    for (auto& tr : m_childsList)
    {
        transformStack.push_back(tr);
    }
    while (!transformStack.empty())
    {
        Transform* curr = transformStack.back();
        transformStack.pop_back();

        curr->m_root = _root;
        for (auto& tr : curr->m_childsList)
        {
            transformStack.push_back(tr);
        }
    }
}

Transform* Transform::Find(std::wstring_view name) const
{
    for (int i = 0; i < childCount; i++)
    {
        if (Transform* child = GetChild(i))
        {
            GameObject& obj = child->gameObject;
            if (obj.name == name)
            {
                return child;
            }     
        }
    }
}