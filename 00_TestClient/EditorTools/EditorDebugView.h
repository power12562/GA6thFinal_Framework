#pragma once
#include "UmFramework.h"

class EditorDebugView
    : public EditorWindow
{
public:
    EditorDebugView();
    virtual ~EditorDebugView();
private:
    virtual void _CALLBACK OnGuiStart() override;

    virtual void _CALLBACK OnPreFrame() override;

    virtual void _CALLBACK OnFrame() override;

    virtual void _CALLBACK OnPostFrame() override;
};

