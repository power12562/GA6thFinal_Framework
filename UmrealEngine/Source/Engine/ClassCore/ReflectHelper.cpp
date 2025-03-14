#include "ReflectHelper.h"
#include "pch.h"

//설정값 관리용 전역 변수 재컴파일 방지를 위해 cpp에 초기값 선언.
using namespace ImGui::InputAutoSetting;

//Int 설정
float InputAutoSettingInt::v_speed = 1.f;
int InputAutoSettingInt::v_min = 0;
int InputAutoSettingInt::v_max = 0;
std::string InputAutoSettingInt::format = "%d";
ImGuiSliderFlags InputAutoSettingInt::flags = 0;

InputAutoSettingInt Int;

//Float 설정
float InputAutoSettingFloat::v_speed = 1.f;
float InputAutoSettingFloat::v_min = 0;
float InputAutoSettingFloat::v_max = 0;
std::string InputAutoSettingFloat::format = "%.3f";
ImGuiSliderFlags InputAutoSettingFloat::flags = 0;

InputAutoSettingFloat Float;

//String 설정
ImGuiInputTextFlags InputAutoSettingString::flags = 0;
ImGuiInputTextCallback InputAutoSettingString::callback = nullptr;
void* InputAutoSettingString::user_data = nullptr;

InputAutoSettingString String;
