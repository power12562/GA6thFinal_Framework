#include "pch.h"

namespace u8_literals
{
    const char* operator"" to_char(const char8_t* str, std::size_t len) 
    {
        return reinterpret_cast<const char*>(str);
    }
}

