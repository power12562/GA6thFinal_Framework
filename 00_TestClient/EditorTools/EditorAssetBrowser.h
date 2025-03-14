#pragma once
#include "UmFramework.h"

class EditorAssetBrowser
    : public EditorWindow
{
public:
    EditorAssetBrowser();
    virtual ~EditorAssetBrowser();
private:
    virtual void _CALLBACK OnGuiStart() override;

    virtual void _CALLBACK OnPreFrame() override;

    virtual void _CALLBACK OnFrame() override;

    virtual void _CALLBACK OnPostFrame() override;
};

