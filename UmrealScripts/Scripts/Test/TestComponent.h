#pragma once
#include "UmFramework.h"
class TestComponent : public Component
{
    USING_PROPERTY(TestComponent)
public:
    TestComponent();
    virtual ~TestComponent();

    REFLECT_FIELDS_BEGIN(Component)
    REFLECT_FIELDS_END(TestComponent, public)

public:
    virtual void Update();
};
