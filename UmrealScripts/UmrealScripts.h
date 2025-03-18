#pragma once
#include "UmFramework.h"

#ifdef UMREALSCRIPTS_EXPORT
#define UMREALSCRIPTS_DECLSPEC __declspec(dllexport)
#else
#define UMREALSCRIPTS_DECLSPEC __declspec(dllimport)
#endif

/// <summary>
/// 스크립트 파일을 자동 생성해주는 함수입니다.
/// </summary>
/// <param name="fileName"></param>
/// <returns></returns>
extern "C" UMREALSCRIPTS_DECLSPEC void CreateNewScript(const char* fileName);

/// <summary>
/// 엔진 코어를 스크립트와 동기화합니다.
/// 모든 스크립트는 코어 사용시 UmrealEngine에 접근해 사용해야 합니다.
/// </summary>
/// <param name="engineCores :">엔진 코어 모음</param>
/// <returns></returns>
extern "C" UMREALSCRIPTS_DECLSPEC void InitalizeUmrealScript(const EngineCores& engineCores);

//컴포넌트 생성 선언용 매크로
#define UMREAL_COMPONENT(CLASS_NAME)                                \
extern "C" UMREALSCRIPTS_DECLSPEC Component* NewTest##CLASS_NAME()  \
{                                                                   \
    return new CLASS_NAME();                                        \
}                                                                

//이 아래는 컴포넌트 생성 함수들을 추가합니다.
//CreateNewScript() 함수에서 직접 수정하기 때문에 주의해야 합니다.


#include "Scripts\TestFolder/TestScript.h"
UMREAL_COMPONENT(TestScript)
