#include "UmrealScripts.h"

extern "C" UMREALSCRIPTS_DECLSPEC Component* NewTestScripts()
{
    return new TestComponent();
}
