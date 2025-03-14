#pragma once
#include "UmFramework.h"

using namespace Microsoft::WRL;

class WindowApp;
extern WindowApp& Application;

class WindowApp : public SimpleWindowClient
{
	friend	int APIENTRY wWinMain(
			_In_ HINSTANCE hInstance,
			_In_opt_ HINSTANCE hPrevInstance,
			_In_ LPWSTR lpCmdLine,
			_In_ int nCmdShow);
public:
    static WindowApp Application;
private:
	WindowApp();
	virtual ~WindowApp() override;
public:
    void ResizeRenderTarget(UINT width, UINT height);
private:
	virtual void PreInitialize();
	virtual void ModuleInitialize();

	virtual void ModuleUnitialize();
	virtual void PreUnitialize();

	virtual void ClientUpdate();
	virtual void ClientRender();

private:
	void InitD311();
	void UninitD311();
	ComPtr<ID3D11Device> m_device;
	ComPtr<ID3D11DeviceContext> m_deviceContext;

	void InitImgui();
	void UninitImgui();
	void ImguiBeginDraw();
	void ImguiEndDraw();

	void InitDXGI();
	void UninitDXGI();
	ComPtr<IDXGIFactory4>   m_DXGIFactory4;
	ComPtr<IDXGISwapChain1> m_SwapChain1;
	ComPtr<ID3D11RenderTargetView> m_backBufferRTV;
};