#pragma once
#include <rfl/json.hpp>
#include <rfl.hpp>
#include <functional>
#include <typeinfo>
#include <ranges>
#include <iostream>

//reflect-cpp 라이브러 docs https://rfl.getml.com/docs-readme/#the-basics
//github https://github.com/getml/reflect-cpp

#define  REFLECT_FIELDS_BASE_BEGIN()						    \
public:                                                         \
struct reflect_struct								            \
{                                 
#define REFLECT_FIELDS_BASE_END(CLASS, fields_access_modifier)	\
};                                                              \
fields_access_modifier##:                                       \
reflect_struct reflect_fields;						            \
                                                                \
public:                                                         \
using ReflectionType = reflect_struct;					        \
CLASS(const ReflectionType& _impl)						        \
: reflect_fields(_impl) {}								        \
const ReflectionType& reflection() const { return reflect_fields; } 

#define  REFLECT_FIELDS_BEGIN(BASE)							    \
using Base = BASE;										        \
struct reflect_struct									        \
{														        \
	rfl::Flatten<BASE::reflect_struct> Basefields{};	 

#define REFLECT_FIELDS_END(CLASS, fields_access_modifier)	    \
REFLECT_FIELDS_BASE_END(CLASS, fields_access_modifier)

//ImGui Inspector helper
namespace ImGui
{	
	namespace InputAutoSetting
	{
		struct InputAutoSettingInt
		{
			static float v_speed;
			static int v_min;
			static int v_max;
			static std::string format;
			static ImGuiSliderFlags flags;
		};
		extern InputAutoSettingInt Int;
		struct InputAutoSettingFloat
		{
			static float v_speed;
			static float v_min;
			static float v_max;
			static std::string format;
			static ImGuiSliderFlags flags;
		};
		extern InputAutoSettingFloat Float;
		struct InputAutoSettingString
		{
			static ImGuiInputTextFlags flags;
			static ImGuiInputTextCallback callback;
			static void* user_data;
		};
		extern InputAutoSettingString String;
	};

    template<class T>
    void InputReflectFields(T& obj);

    template<class T>
    void InputReflectFields(T* obj);
}



//구현부
namespace ImGui
{
    namespace Private
    {
        template<class T>
        bool InputAuto(T field)
        {
            const type_info& type = typeid(*field.value());
            const auto& name = field.name();
            auto* value = field.value();

            auto NotArrayTypeFunc =
                [](auto* value, const char* name)
                {
                    using OriginType = std::remove_cvref_t<decltype(*value)>;

                    using namespace InputAutoSetting;
                    bool isEdit = false;

                    if constexpr (std::is_same_v<OriginType, int>)
                    {
                        int* val = (int*)value;
                        isEdit = ImGui::DragInt(name, val,
                            Int.v_speed,
                            Int.v_min,
                            Int.v_max,
                            Int.format.c_str(),
                            Int.flags);
                    }
                    else if constexpr (std::is_same_v<OriginType, float>)
                    {
                        float* val = (float*)value;
                        isEdit = ImGui::DragFloat(name, val,
                            Float.v_speed,
                            Float.v_min,
                            Float.v_max,
                            Float.format.c_str(),
                            Float.flags);
                    }
                    else if constexpr (std::is_same_v<OriginType, bool>)
                    {
                        bool* val = (bool*)value;
                        isEdit = ImGui::Checkbox(name, value);
                    }
                    else if constexpr (std::is_same_v<OriginType, std::string>)
                    {
                        std::string* val = (std::string*)value;
                        isEdit = ImGui::InputText(name, val,
                            String.flags,
                            String.callback,
                            String.user_data);
                    }
                    return isEdit;
                };

            auto ArrayTypeFunc =
                [&name, &value, &NotArrayTypeFunc](const type_info& type)
                {
                    using OriginType = std::remove_cvref_t<decltype(*value)>;

                    bool isEdit = false;
                    if constexpr (type_utils::is_std_array_v<OriginType>)
                    {
                        if (ImGui::CollapsingHeader((const char*)name.data()))
                        {
                            if constexpr (std::ranges::range<decltype(*value)>)
                            {
                                int i = 0;
                                for (auto& val : *value)
                                {
                                    isEdit = NotArrayTypeFunc(&val, std::format("[{}]", i).c_str());
                                    i++;
                                }
                            }
                        }
                    }
                    else if constexpr (type_utils::is_std_vector_v<OriginType>)
                    {
                        if constexpr (std::ranges::range<decltype(*value)>)
                        {
                            if (ImGui::CollapsingHeader((const char*)name.data()))
                            {
                                bool isEdit = false;
                                int i = 0;
                                for (auto& val : *value)
                                {
                                    isEdit = NotArrayTypeFunc(&val, std::format("[{}]", i).c_str());
                                    i++;
                                }
                                if (ImGui::Button("+"))
                                {
                                    value->emplace_back();
                                }
                                ImGui::SameLine();
                                if (ImGui::Button("-"))
                                {
                                    value->pop_back();
                                }
                            }
                        }
                    }
                    return isEdit;
                };

            bool isEdit = false;
            ImGui::PushID(typeid(T).hash_code());
            {
                if (NotArrayTypeFunc(value, name.data()))
                {
                    isEdit = true;
                }
                else if (ArrayTypeFunc(type))
                {
                    isEdit = true;
                }
            }
            ImGui::PopID();
            return isEdit;
        }
    }

    template<class T>
    void InputReflectFields(T& obj)
    {
        const auto view = rfl::to_view(obj.reflect_fields);
        view.apply([](auto& f)
            {
                ImGui::Private::InputAuto(f);
            });
    }

    template<class T>
    void InputReflectFields(T* obj)
    {
        const auto view = rfl::to_view(obj->reflect_fields);
        view.apply([](auto& f)
            {
                ImGui::Private::InputAuto(f);
            });
    }
}

