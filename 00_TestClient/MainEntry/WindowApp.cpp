#include "pch.h"
#include "WindowApp.h"

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    WindowApp app;
    app.Initialize(hInstance);
    app.Run();
    app.Uninitialize();

    return 0;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

WindowApp::WindowApp()
{
    this->SetStyleToWindowed();
    this->clientSize = { 1920, 1080 };
    this->windowName = L"TestClient";
    this->customWndProc = WndProc;
}
WindowApp::~WindowApp() = default;


void WindowApp::PreInitialize()
{

}

void WindowApp::ModuleInitialize()
{
    InitD311();
    InitImgui();
    InitDXGI();
}


void WindowApp::PreUnitialize()
{

}

void WindowApp::ModuleUnitialize()
{
    UninitDXGI();
    UninitImgui();
    UninitD311();
}

void WindowApp::ClientUpdate()
{

}

void WindowApp::ClientRender()
{

}

void WindowApp::InitD311()
{
    UINT flags = 0;
#ifdef _DEBUG
    flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D11CreateDevice(
        NULL, 
        D3D_DRIVER_TYPE_HARDWARE, 
        NULL, 
        flags, 
        NULL, 
        NULL, 
        D3D11_SDK_VERSION, 
        &m_device, 
        NULL, 
        &m_deviceContext
    );
}

void WindowApp::UninitD311()
{

}

void WindowApp::InitImgui()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch
    ImGui_ImplWin32_Init(this->GetHWND());
    ImGui_ImplDX11_Init(this->m_device.Get(), this->m_deviceContext.Get());
}

void WindowApp::UninitImgui()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void WindowApp::InitDXGI()
{
    CreateDXGIFactory1(__uuidof(IDXGIFactory4), &m_DXGIFactory4);
    DXGI_SWAP_CHAIN_DESC1 swapDesc{};
    swapDesc.Width = clientSize.cx;
    swapDesc.Height = clientSize.cy;
    swapDesc.BufferCount = 2; //버퍼 개수
    swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; //버퍼 사용 방식 지정
    swapDesc.SampleDesc.Count = 1;
    swapDesc.SampleDesc.Quality = 0;
    swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL; //플립 모드 사용.   
    swapDesc.Flags |= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; //전체 화면 변환시 해상도 및 모니터 설정 자동 변경 플래그
    swapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //출력 포멧 지정.

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC screenDesc{};
    screenDesc.Windowed = true; //창모드 유무
    screenDesc.RefreshRate.Numerator = 0;
    screenDesc.RefreshRate.Denominator = 0;

    m_DXGIFactory4->CreateSwapChainForHwnd(m_device.Get(), GetHWND(), &swapDesc, &screenDesc, nullptr, &m_SwapChain1);
}

void WindowApp::UninitDXGI()
{

}
