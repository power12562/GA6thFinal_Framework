#pragma once

namespace u8_literals
{
    //u8 타입 const char* 캐스팅용 리터럴
    const char* operator"" to_char(const char8_t* str, std::size_t len);
}
