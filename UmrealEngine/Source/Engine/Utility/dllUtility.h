#pragma once

namespace Utility
{
    /// <summary>
    /// 로드된 DLL 함수의 이름들을 반환합니다.
    /// </summary>
    /// <param name="dllModule :">DLL 핸들</param>
    /// <returns></returns>
    std::vector<std::string> GetDLLFuntionNameList(HMODULE dllModule);

    /// <summary>
    /// 배치파일을 실행 합니다.
    /// *반드시 경로를 표현할때 "\\" 사용하세요. "/" 사용시 실패함. 
    /// </summary>
    /// <returns>성공 여부</returns>
    bool RunBatchFile(const std::wstring_view BatchFilePath);
}