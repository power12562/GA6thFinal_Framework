#include "Scripts.h"
#include "UmrealScripts.h"

UMREALSCRIPTS_DECLSPEC void InitalizeUmrealScript(const EngineCores& engineCores)
{
    UmrealEngine = std::make_unique<EngineCores>(engineCores);  //코어 동기화
    ImGui::SetCurrentContext(&engineCores.Imguicontext);        //Imguicontext 동기화
}

