#pragma once
#undef max
#undef min
#include <rfl/json.hpp>
#include <rfl.hpp>

//reflect-cpp 라이브러리 docs https://rfl.getml.com/docs-readme/#the-basics
//reflect-cpp github https://github.com/getml/reflect-cpp

#define  REFLECT_FIELDS_BASE_BEGIN()						        \
public:                                                             \
struct reflect_struct								                \
{                          

#define REFLECT_FIELDS_BASE_END(CLASS, fields_access_modifier)	    \
};                                                                  \
fields_access_modifier##:                                           \
reflect_struct reflect_fields;						                \
                                                                    \
public:                                                             \
using ReflectionType = reflect_struct;					            \
CLASS(const ReflectionType& _impl)						            \
: reflect_fields(_impl) {}								            \
const ReflectionType& reflection() const { return reflect_fields; } \
virtual void imgui_draw_reflect_fields()                            \
{                                                                   \
    ImGui::PushID(typeid(*this).hash_code());                       \
    ImGui::InputReflectFields(this);                                \
    ImGui::PopID();                                                 \
}                                                                   \
virtual std::string serialized_reflect_fields()                     \
{                                                                   \
    return json::SerializedObjet(this);                             \
}

#define  REFLECT_FIELDS_BEGIN(BASE)							        \
using Base = BASE;										            \
struct reflect_struct									            \
{														            \
	rfl::Flatten<BASE::reflect_struct> Basefields{};	 

#define REFLECT_FIELDS_END(CLASS, fields_access_modifier)	        \
REFLECT_FIELDS_BASE_END(CLASS, fields_access_modifier)

template <typename, typename = std::void_t<>>
struct has_reflection : std::false_type {};

template <typename T>
struct has_reflection<T, std::void_t<decltype(std::declval<T>().reflection())>> : std::true_type {};

template <typename T>
constexpr bool has_reflection_v = has_reflection<T>::value;

//ImGui Inspector helper
namespace ImGui
{	
	namespace InputAutoSetting
	{
		struct InputAutoSettingInt
		{
			static int step;
			static int step_fast;
			static ImGuiInputTextFlags flags;
		};
		extern InputAutoSettingInt Int;

		struct InputAutoSettingFloat
		{
			static float step;
			static float step_fast;
            static std::string format;
			static ImGuiInputTextFlags flags;
		};
		extern InputAutoSettingFloat Float;

        struct InputAutoSettingDouble
        {
            static double step;
            static double step_fast;
            static std::string format;
            static ImGuiInputTextFlags flags;
        };
        extern InputAutoSettingDouble Double;

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


//serialized helper
namespace json
{
    template<typename Type>
    std::string SerializedObjet(Type& obj);

    template<typename Type>
    std::string SerializedObjet(Type* obj);
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
                    using namespace InputAutoSetting;
                    using value_type = std::remove_cvref_t<decltype(*value)>;
                    using OriginType = type_utils::get_field_type_t<value_type>;
                
                    bool isEdit = false;
                    auto& val = *value;

                    if constexpr (std::is_same_v<OriginType, int>)
                    {                       
                        int input = val;
                        isEdit = ImGui::InputInt(name, &input,
                            Int.step,
                            Int.step_fast,
                            Int.flags);

                        if (isEdit && ImGui::IsItemDeactivatedAfterEdit())
                        {
                            val = input;
                        }
                    }
                    else if constexpr (std::is_same_v<OriginType, float>)
                    {
                        float input = val;
                        isEdit = ImGui::InputFloat(name, &input,
                            Float.step,
                            Float.step_fast,
                            Float.format.c_str(),
                            Float.flags);

                        if (isEdit && ImGui::IsItemDeactivatedAfterEdit())
                        {
                            val = input;
                        }
                    }
                    else if constexpr (std::is_same_v<OriginType, double>)
                    {
                        double input = val;
                        isEdit = ImGui::InputDouble(name, &input,
                            Float.step,
                            Float.step_fast,
                            Float.format.c_str(),
                            Float.flags);

                        if (isEdit && ImGui::IsItemDeactivatedAfterEdit())
                        {
                            val = input;
                        }
                    }
                    else if constexpr (std::is_same_v<OriginType, bool>)
                    {
                        bool input = val;
                        isEdit = ImGui::Checkbox(name, &input);

                        if (isEdit && ImGui::IsItemDeactivatedAfterEdit())
                        {
                            val = input;
                        }
                    }
                    else if constexpr (std::is_same_v<OriginType, std::string>)
                    {
                        static std::string input;
                        input = val;
                        isEdit = ImGui::InputText(name, &input,
                            String.flags,
                            String.callback,
                            String.user_data);

                        if (isEdit && ImGui::IsItemDeactivatedAfterEdit())
                        {
                            val = input;
                        }
                    }
                    else if constexpr (std::is_same_v<OriginType, std::wstring>)
                    {
                        static std::string input;
                        input = wstring_to_u8(val);
                        isEdit = ImGui::InputText(name, &input,
                            String.flags,
                            String.callback,
                            String.user_data);

                        if (isEdit && ImGui::IsItemDeactivatedAfterEdit())
                        {
                            val = u8_to_wstring(input);
                        }
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


namespace json
{
    template<typename Type>
    inline std::string SerializedObjet(Type& obj)
    {
        static_assert(has_reflection_v<Type>, "Type cannot be used for reflection.");


        return rfl::json::write(obj);
    }

    template<typename Type>
    inline std::string SerializedObjet(Type* obj)
    {
        return SerializedObjet(*obj);
    }
}
