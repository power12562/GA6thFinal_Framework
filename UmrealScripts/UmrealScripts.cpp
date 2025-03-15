#include "Scripts.h"
#include "UmrealScripts.h"

UMREALSCRIPTS_DECLSPEC void InitalizeScript(const EngineCores& engineCores)
{
    ImGui::SetCurrentContext(engineCores.imguiContext);
    UmrealEngine = std::make_unique<EngineCores>(engineCores);
}

UMREALSCRIPTS_DECLSPEC Component* NewTestScript()
{
    return new TestComponent();
}
