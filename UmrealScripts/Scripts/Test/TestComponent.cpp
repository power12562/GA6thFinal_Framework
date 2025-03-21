#include "TestComponent.h"
TestComponent::TestComponent() = default;
TestComponent::~TestComponent() = default;

void TestComponent::Test()
{
    float a = this->Out;
    this->Out += 1.5;
    this->Out -= 1.5;
}

void TestComponent::Update()
{
    Test();
}
