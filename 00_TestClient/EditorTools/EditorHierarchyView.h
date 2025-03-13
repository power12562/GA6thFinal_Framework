#pragma once
#include "UmFramework.h"

class EditorHierarchyView
    : public EditorWindow
{
public:
    EditorHierarchyView();
    virtual ~EditorHierarchyView();
private:
    virtual void _CALLBACK OnGuiStart() override;

    virtual void _CALLBACK OnPreFrame() override;

    virtual void _CALLBACK OnFrame() override;

    virtual void _CALLBACK OnPostFrame() override;
};

