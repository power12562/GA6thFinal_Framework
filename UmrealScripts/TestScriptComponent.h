#pragma once
#include "UmFramework.h"

class TestComponent : public Component
{
public:
    TestComponent() {};
    virtual ~TestComponent() override {};
public:
    REFLECT_FIELDS_BEGIN(Component)
    //std::string dll_rfl_string{};
    //std::array<bool, 5> dll_rfl_bools{};
    //std::vector<int> dll_rfl_ints{};
    //std::string dll_test_string{ "runtime add" };
    REFLECT_FIELDS_END(TestComponent, public)
public:
    USING_PROPERTY(TestComponent)
    
public:
    virtual void FixedUpdate() override;
    virtual void Update() override;
};