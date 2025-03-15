#include "TestScriptComponent.h"
#include "ScriptsFramework.h"

void TestComponent::FixedUpdate()
{

}

void TestComponent::Update()
{
    ImGui::Text((const char*)u8"테스트 컴포넌트 업데이트 dll 에서 호출 테스트 중임");
    TimeSystem& Time = UmrealEngine->Time;
    ImGui::InputDouble("time scale", &Time.timeScale);

    ImGui::Text("time : %f", Time.time());
    ImGui::Text("realtimeSinceStartup : %f", Time.realtimeSinceStartup());

    ImGui::Text("frameCount : %llu", Time.frameCount());

    ImGui::Text("FPS : %d", Time.frameRate());
    ImGui::Text("DeltaTime : %f", Time.deltaTime());

    ImGui::Text("unscaledDeltaTime : %f", Time.unscaledDeltaTime());

    ImGui::InputDouble("Fixed Time Step", &Time.fixedTimeStep);
    ImGui::Text("fixedDeltaTime %f", Time.fixedDeltaTime());
    ImGui::Text("fixedUnscaledDeltaTime %f", Time.fixedUnscaledDeltaTime());

    ImGui::InputDouble("maximumDeltaTime", &Time.maximumDeltaTime);
}
