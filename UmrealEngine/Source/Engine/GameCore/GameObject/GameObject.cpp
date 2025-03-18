#include "pch.h"

void GameObject::DontDestroyOnLoad(GameObject& gameObject)
{

}

void GameObject::Instantiate(GameObject& gameObject)
{

}

void GameObject::Destroy(Component& component, float t)
{

}

void GameObject::Destroy(GameObject& gameObject, float t)
{

}

GameObject::GameObject()
{

}

GameObject::~GameObject()
{
    m_components.clear();
}


