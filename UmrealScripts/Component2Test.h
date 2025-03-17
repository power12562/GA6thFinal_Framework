#pragma once
#include "UmFramework.h"

class Component2Test : public Component
{
public:
    Component2Test() {};
    virtual ~Component2Test() override {};
public:
    REFLECT_FIELDS_BEGIN(Component)
    std::string component_name = "Component2Test";
    REFLECT_FIELDS_END(Component2Test, public)
public:
    USING_PROPERTY(Component2Test)

public:
    virtual void FixedUpdate() override;
    virtual void Update() override;
};