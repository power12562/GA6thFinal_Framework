#include "TestComponent.h"
TestComponent::TestComponent() = default;
TestComponent::~TestComponent() = default;

void TestComponent::Test(std::string_view str)
{
    reflect_fields.log.push_back(str.data());
    scrollToBottom = true;
}

void TestComponent::Update()
{
    using namespace u8_literals;
    ImGui::Begin(u8"로그 테스트"_c_str);

    if (ImGui::Button("Clear"))
    {
        reflect_fields.log.clear();
    }

    ImGui::Separator();

    ImGui::BeginChild("LogScroll", ImVec2(0, 300), true, ImGuiWindowFlags_HorizontalScrollbar);

    for (const auto& log : reflect_fields.log)
    {
        ImGui::TextUnformatted(log.c_str());
    }

    if (scrollToBottom)
    {
        ImGui::SetScrollHereY(1.0f);
        scrollToBottom = false;
    }

    ImGui::EndChild();
    ImGui::End();
}

void TestComponent::Awake()
{
    Test("Awake!");
}

void TestComponent::Start()
{
    Test("Start!");
}

void TestComponent::OnEnable()
{
    Test("OnEnable!");
}

void TestComponent::OnDisable()
{
    Test("OnDisable!");
}