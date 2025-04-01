#pragma once
#include "UmFramework.h"

class EditorSceneView
    : public EditorTool
{
public:
    EditorSceneView();
    virtual ~EditorSceneView();
private:
    virtual void _CALLBACK OnGuiStart() override;

    virtual void _CALLBACK OnPreFrame() override;

    virtual void _CALLBACK OnFrame() override;

    virtual void _CALLBACK OnPostFrame() override;
};

