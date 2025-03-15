#pragma once

namespace Utility
{
    /*로드된 DLL 함수 이름들을 반환합니다.*/
    std::vector<std::string> GetDLLFuntionNameList(HMODULE dllModule);
}