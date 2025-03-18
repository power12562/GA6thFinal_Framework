#pragma once
#include "UmFramework.h"
class MyFirstScript : public Component
{
    USING_PROPERTY(MyFirstScript)
public:
    MyFirstScript();
    virtual ~MyFirstScript();
    
    REFLECT_FIELDS_BEGIN(Component)
    [[SerializeField]]
    int a = 10;
    REFLECT_FIELDS_END(MyFirstScript, public)
};
