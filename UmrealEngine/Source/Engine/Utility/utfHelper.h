#pragma once

namespace u8_literals
{
    //u8 타입 const char* 캐스팅용 리터럴
    const char* operator"" to_char(const char8_t* str, std::size_t len);
}

std::wstring u8_to_wstring(std::string_view utf8_str);

std::string wstring_to_u8(std::wstring_view wstring);