#pragma once
#include "UmFramework.h"

using namespace Microsoft::WRL;
class WindowApp : public Application
{
	friend	int APIENTRY wWinMain(
			_In_ HINSTANCE hInstance,
			_In_opt_ HINSTANCE hPrevInstance,
			_In_ LPWSTR lpCmdLine,
			_In_ int nCmdShow);
public:
	WindowApp(HINSTANCE hInstance);
	virtual ~WindowApp() override;

private:
    virtual void ClientUpdate();
    virtual void ClientRender();

    struct D311Module : public IAppModule
    {
        inline static WindowApp* App = nullptr;
        virtual void PreInitialize() {}
        virtual void ModuleInitialize();
        virtual void ModuleUnInitialize();
        virtual void PreUnInitialize() {}
        virtual long InitOrder() { return 0; }
    };
	void InitD311();
	void UninitD311();

	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;

    struct ImguiModule : public IAppModule
    {
        inline static WindowApp* App = nullptr;
        virtual void PreInitialize() {}
        virtual void ModuleInitialize();
        virtual void ModuleUnInitialize();
        virtual void PreUnInitialize() {}
        virtual long InitOrder() { return 1; }
    };
	void InitImgui();
	void UninitImgui();

	void ImguiBeginDraw();
	void ImguiEndDraw();

    struct DXGIModule : public IAppModule
    {
        inline static WindowApp* App = nullptr;
        virtual void PreInitialize() {}
        virtual void ModuleInitialize();
        virtual void ModuleUnInitialize();
        virtual void PreUnInitialize() {}
        virtual long InitOrder() { return 2; }
    };
	void InitDXGI();
	void UninitDXGI();

	ComPtr<IDXGIFactory4>   m_DXGIFactory4;
	ComPtr<IDXGISwapChain1> m_SwapChain1;
	ComPtr<ID3D11RenderTargetView> m_backBufferRTV;
};

    
