#pragma once

class EditorMainToolBar
{
public:
    EditorMainToolBar();
    virtual ~EditorMainToolBar();
public:
    void OnDrawGui();
private:
    void NewProject();
    void SaveProject();
    void OpenProject();
};

