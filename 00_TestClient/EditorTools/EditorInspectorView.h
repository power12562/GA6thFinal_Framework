#pragma once
#include "UmFramework.h"

class EditorInspectorView
    : public EditorWindow
{
public:
    EditorInspectorView();
    virtual ~EditorInspectorView();
private:
    virtual void _CALLBACK OnGuiStart() override;

    virtual void _CALLBACK OnPreFrame() override;

    virtual void _CALLBACK OnFrame() override;

    virtual void _CALLBACK OnPostFrame() override;
};

