#pragma once
#include "UmFramework.h"

// 상단 메뉴 바
class EditorMainMenuBar
    : public EditorMenuBar
{
public:
    EditorMainMenuBar();
    virtual ~EditorMainMenuBar();
private:
    virtual void _CALLBACK OnGuiStart() override;

    virtual void _CALLBACK OnPreFrame() override;

    virtual void _CALLBACK OnFrame() override;

    virtual void _CALLBACK OnPostFrame() override;
};

