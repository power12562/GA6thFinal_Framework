#pragma once

namespace GUI
{
    enum eDirection {
        LEFT, LEFTUP, LEFTDOWN,
        RIGHT, RIGHTUP, RIGHTDOWN,
        UP,
        DOWN,
        CENTER,
    };
    /*
    호버링시 둘팁을 여는 텍스트를 생성
    desc = 툴팁에 나타날 텍스트
    mark = 호버링용 텍스트 글씨
    */
    static void TooltipMarker(const char* desc, const char* mark = "(?)")
    {
        ImGui::TextDisabled(mark);
        if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
        {
            ImGui::BeginTooltip();
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(desc);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    /*
    정렬 텍스트
    *주의: scale값은 기존 폰트를 확장시키므로 텍스트에 계단현상이 발생할 수 있음.
    */
    static void AlignedText(const char* text, eDirection dir = LEFT, float scale = 1.0f)
    {
        if (scale > 0)
        {
            ImGuiWindow* window = ImGui::GetCurrentWindow();
            float old = window->FontWindowScale;
            ImGui::SetWindowFontScale(scale);

            float windowWidth = ImGui::GetWindowSize().x;
            float textWidth = ImGui::CalcTextSize(text).x;
            float weight = 0.0f;

            switch (dir)
            {
            case GUI::LEFT:
                weight = 0.0f;
                break;
            case GUI::RIGHT:
                weight = 1.0f;
                break;
            case GUI::CENTER:
                weight = 0.5f;
                break;
            default:
                break;
            }

            // 정렬에 맞게 X 위치를 조정
            ImGui::SetCursorPosX((windowWidth - textWidth) * weight);
            ImGui::Text("%s", text);

            ImGui::SetWindowFontScale(old);
        }
    }

    /*
    여백있는 구분선
    */
    static void Separator(float upPadding, float downPadding)
    {
        ImGui::Dummy(ImVec2(0.0f, upPadding));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, downPadding));
    }
    static void Separator(float spacing = 5.0f)
    {
        ImGui::Dummy(ImVec2(0.0f, spacing));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0.0f, spacing));
    }

    /*
    토글이 가능한 버튼 (false->true / true->false)
    return: 버튼을 눌렀다 뗄 때
    */
    static bool ToggleButton(const char* label, bool* v, ImVec4 trueColor, ImVec4 falseColor)
    {
        if (v)
        {
            if (*v)
                ImGui::PushStyleColor(ImGuiCol_Button, trueColor); // 활성화 색상
            else
                ImGui::PushStyleColor(ImGuiCol_Button, falseColor); // 비활성화 색상

            bool clicked = ImGui::Button(label);
            if (clicked)
                *v = !*v;

            ImGui::PopStyleColor();
            return clicked;
        }
        return false;
    }

    /*
    로딩
    */
    static void LoadingSpinner(float radius = 10.0f, ImVec4 color = ImVec4(1, 1, 1, 1))
    {
        static int segments = 20;
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        ImVec2 center = ImGui::GetCursorScreenPos();
        center.x += radius;
        center.y += radius;

        float start = (float)ImGui::GetTime() * 3.0f;
        float angle_offset = 2.0f * IM_PI / segments;

        for (int i = 0; i < segments - 4; i++)
        {
            float angle = start + (i * angle_offset);
            ImVec2 p1 = ImVec2(center.x + cosf(angle) * radius, center.y + sinf(angle) * radius);
            ImVec2 p2 = ImVec2(center.x + cosf(angle + angle_offset) * radius, center.y + sinf(angle + angle_offset) * radius);
            draw_list->AddLine(p1, p2, ImGui::GetColorU32(color), 2.0f);
        }
    }

    // Make the UI compact because there are so many fields
    static void PushStyleCompact(float _val = 0.60f)
    {
        ImGuiStyle& style = ImGui::GetStyle();
        ImGui::PushStyleVarY(ImGuiStyleVar_FramePadding, (float)(int)(style.FramePadding.y * _val));
        ImGui::PushStyleVarY(ImGuiStyleVar_ItemSpacing, (float)(int)(style.ItemSpacing.y * _val));
    }
    static void PopStyleCompact()
    {
        ImGui::PopStyleVar(2);
    }

    /*
    색상 선택 버튼
    */
    static bool ColorPickerButton(const char* label, ImVec4* color)
    {
        bool colorChanged = false;
        // 버튼에 현재 색상을 표시
        if (ImGui::ColorButton(label, *color))
        {
            ImGui::OpenPopup(label);
        }
        if (ImGui::BeginPopup(label))
        {
            // 기본 색상 편집기를 팝업 내에 표시
            colorChanged = ImGui::ColorPicker4("##picker", (float*)color);
            ImGui::EndPopup();
        }
        return colorChanged;
    }
}
