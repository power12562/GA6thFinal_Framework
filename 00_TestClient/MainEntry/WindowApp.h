#pragma once
#include "UmFramework.h"

using namespace Microsoft::WRL;

class WindowApp : public SimpleWindowClient
{
	friend	int APIENTRY wWinMain(
			_In_ HINSTANCE hInstance,
			_In_opt_ HINSTANCE hPrevInstance,
			_In_ LPWSTR lpCmdLine,
			_In_ int nCmdShow);
public:
	WindowApp();
	virtual ~WindowApp() override;

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

    float testFloat = 10.f;
public:
    REFLECT_FIELDS_BASE_BEGIN()
    float rflFloat = 1.0f;
    double rflDouble = 1.333;
    int rflInt = 1;
    bool rflBool = false;
    std::array<bool, 10> rflArrayBool{};
    std::vector<float> rflVectorFloat{};
    REFLECT_FIELDS_BASE_END(WindowApp, public)

public:
    USING_PROPERTY(WindowApp)

    GETTER(float, Float)
    {
        return reflect_fields.rflFloat;
    }
    SETTER(float, Float)
    {
        reflect_fields.rflBool = !reflect_fields.rflBool;
        reflect_fields.rflFloat = value;
    }
    PROPERTY(Float)
};

    
