#include "TestComponent.h"
TestComponent::TestComponent() = default;
TestComponent::~TestComponent() = default;

void TestComponent::Update()
{
    using namespace u8_literals;

    ImGui::Begin(u8"이건 TestComponent의 업데이트 함수가 호출되서 나오는거임."to_char);
    {

    }
    ImGui::End();
}
