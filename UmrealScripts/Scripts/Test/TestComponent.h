﻿#pragma once
#include "UmFramework.h"
class TestComponent : public Component
{
    USING_PROPERTY(TestComponent)
public:
    TestComponent();
    virtual ~TestComponent();

    REFLECT_FIELDS_BEGIN(Component)
    //int testint = 10;
    //float floatTest = 1.f;
    //std::string testString = "asdas";
    //std::wstring testWstring = L"";
    //std::array<int, 10> doubleArray{1,2,3,4,5,6,7,8,9, 10};
    std::vector<std::string> log{};
    REFLECT_FIELDS_END(TestComponent, public)

private:
    float aa = 1.5;
    bool scrollToBottom = false;
public:
    GETTER(float, Out)
    {
        return aa;
    }
    SETTER(float, Out)
    {
        aa = value;
    }
    PROPERTY(Out);

public:
    void Test(std::string_view str);
    virtual void Awake(); 
    virtual void Start();
    virtual void OnEnable();
    virtual void OnDisable();
    virtual void Update() override;

};