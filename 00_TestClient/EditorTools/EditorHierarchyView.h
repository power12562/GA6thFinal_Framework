#pragma once
#include "UmFramework.h"

class EditorHierarchyView
    : public EditorTool
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

class SampleObject : public IEditorObject
{
public:
    SampleObject()
    {
        File::CreateFileID(&mID);
    }
    virtual void _CALLBACK OnDrawInspectorView() override {
        ImGui::Text(mID.c_str());
    }
private:
    File::ID mID;
};