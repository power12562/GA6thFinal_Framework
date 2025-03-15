#include "pch.h"

std::vector<std::string> Utility::GetDLLFuntionNameList(HMODULE dllModule)
{
    std::vector<std::string> out;

    if (!dllModule) return out;  

    auto dosHeader = (PIMAGE_DOS_HEADER)dllModule;
    if (dosHeader->e_magic != IMAGE_DOS_SIGNATURE) return out;

    auto ntHeaders = (PIMAGE_NT_HEADERS)((BYTE*)dllModule + dosHeader->e_lfanew);
    if (ntHeaders->Signature != IMAGE_NT_SIGNATURE) return out;

    auto exportDirectoryRVA = ntHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
    if (!exportDirectoryRVA) return out;

    auto exportDirectory = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)dllModule + exportDirectoryRVA);

    auto namesRVA = (DWORD*)((BYTE*)dllModule + exportDirectory->AddressOfNames);
    if (!namesRVA)
    {
        return out;
    }

    for (DWORD i = 0; i < exportDirectory->NumberOfNames; i++) 
    {
        const char* functionName = (const char*)((BYTE*)dllModule + namesRVA[i]); 
        if (!IsBadReadPtr(functionName, 1)) 
        {
            out.push_back(functionName);
        }
    }

    return out;
}
