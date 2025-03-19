#include "pch.h"

namespace u8_literals
{
    const char* operator"" to_char(const char8_t* str, std::size_t len) 
    {
        return reinterpret_cast<const char*>(str);
    }
}

std::wstring u8_to_wstring(std::string_view utf8_str)
{
    if (utf8_str.empty()) return std::wstring();

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, utf8_str.data(), -1, nullptr, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8_str.data(), -1, &wstr[0], size_needed);
    wstr.pop_back();
    return wstr;
}

std::string wstring_to_u8(std::wstring_view wstring)
{
    if (wstring.empty()) return std::string();

    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstring.data(), -1, nullptr, 0, nullptr, nullptr);
    std::string utf8_str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstring.data(), -1, &utf8_str[0], size_needed, nullptr, nullptr);

    utf8_str.pop_back(); 
    return utf8_str;
}
