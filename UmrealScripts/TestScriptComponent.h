#pragma once
#include "UmFramework.h"
class TestComponent : public Component
{
public:
    TestComponent() {};
    virtual ~TestComponent() override {};
public:
    REFLECT_FIELDS_BEGIN(Component)

    REFLECT_FIELDS_END(TestComponent, public)
public:
    USING_PROPERTY(TestComponent)
    
public:
    virtual void FixedUpdate() override;
    virtual void Update() override;
};