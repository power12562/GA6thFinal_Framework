#include "TestScriptComponent.h"
#include "UmFramework.h"

void TestComponent::FixedUpdate()
{

}

void TestComponent::Update()
{
    ImGui::Text((const char*)u8"이건 TestComponent의 Update 입니다.");
}
