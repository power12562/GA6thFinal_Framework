#pragma once
#include "UmFramework.h"

class EditorInspectorView
    : public EditorTool
{
public:
    EditorInspectorView();
    virtual ~EditorInspectorView();
private:
    virtual void _CALLBACK OnGuiStart() override;

    virtual void _CALLBACK OnPreFrame() override;

    virtual void _CALLBACK OnFrame() override;

    virtual void _CALLBACK OnPostFrame() override;
public:
    void SetFocusObject(std::weak_ptr<IEditorObject> obj);
private:
    std::weak_ptr<IEditorObject> mFocusedObject;
};

