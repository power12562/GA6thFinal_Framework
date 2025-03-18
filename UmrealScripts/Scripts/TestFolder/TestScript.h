#pragma once
#include "UmFramework.h"
class TestScript : public Component
{
    USING_PROPERTY(TestScript)
public:
    TestScript();
    virtual ~TestScript();

    REFLECT_FIELDS_BEGIN(Component)
    REFLECT_FIELDS_END(TestScript, public)
};
