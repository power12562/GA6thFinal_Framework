#include "pch.h"
#include "WindowApp.h"

/* 에디터 툴 */
#include "EditorTools/EditorDockSpace.h"
#include "EditorTools/EditorDebugView.h"
#include "EditorTools/EditorMainMenuBar.h"
#include "EditorTools/EditorHierarchyView.h"
#include "EditorTools/EditorInspectorView.h"
#include "EditorTools/EditorSceneView.h"
#include "EditorTools/EditorAssetBrowser.h"

WindowApp WindowApp::Application;
WindowApp& Application = WindowApp::Application;

//??
int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    Application.Initialize(hInstance);
    Application.Run();
    Application.Uninitialize();

    return 0;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
    {
        // 리사이즈 렌더타겟 
        Application.ResizeRenderTarget(LOWORD(lParam), HIWORD(lParam));
        break;
    }
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
    // 기본 모니터의 가로, 세로 해상도 얻기
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    this->clientSize = { screenWidth, screenHeight };
    //this->clientSize = { 1920, 1080 };

    this->windowName = L"TestClient";
    this->customWndProc = WndProc;

    this->windowStyleEX = WS_OVERLAPPEDWINDOW;
}
WindowApp::~WindowApp() = default;

void WindowApp::ResizeRenderTarget(UINT width, UINT height)
{
    if (m_SwapChain1 && m_deviceContext)
    {
        D3D11_VIEWPORT viewport = {};
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.Width = static_cast<float>(width);
        viewport.Height = static_cast<float>(height);
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;

        m_backBufferRTV.Reset();

        HRESULT hr = m_SwapChain1->ResizeBuffers(0, // 현재 개수 유지
            (UINT)viewport.Width, // 해상도 변경
            (UINT)viewport.Height,
            DXGI_FORMAT_UNKNOWN, // 현재 포맷 유지
            0);

        // Get backbuffer by Swapchain
        ComPtr<ID3D11Texture2D> backBufferTex = nullptr;
        hr = m_SwapChain1->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(backBufferTex.GetAddressOf()));
        D3D11_TEXTURE2D_DESC desc = {};
        backBufferTex->GetDesc(&desc);
        // Create RenderTargetView
        if (backBufferTex)
        {
            hr = m_device->CreateRenderTargetView(backBufferTex.Get(), nullptr, m_backBufferRTV.GetAddressOf());
            if (FAILED(hr))
            {
                assert(false, "HResult Failed To CreateRenderTargetView");
            }
        }
        m_deviceContext->RSSetViewports(1, &viewport);
    }
   
}


void WindowApp::PreInitialize()
{
    /* 에디터 툴 초기화 */
    Editor.PushTool(new EditorDockSpace);
    Editor.PushTool(new EditorMainMenuBar);
    Editor.PushTool(new EditorDebugView);
    Editor.PushTool(new EditorHierarchyView);
    Editor.PushTool(new EditorInspectorView);
    Editor.PushTool(new EditorSceneView);
    Editor.PushTool(new EditorAssetBrowser);
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
    TimeSystem::Engine::TimeSystemUpdate();
    while (TimeSystem::Engine::TimeSystemFixedUpdate())
    {

    }
}

void WindowApp::ClientRender()
{
    constexpr float clearColor[] = { 0.f,0.f,0.f,1.f };
    m_deviceContext->ClearRenderTargetView(m_backBufferRTV.Get(), clearColor);
    m_deviceContext->OMSetRenderTargets(1, m_backBufferRTV.GetAddressOf(), nullptr);
    
    Editor.DrawGui();

    m_SwapChain1->Present(1, 0);
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
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui::StyleColorsDark();

    ImFontConfig fontConfig{};
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 20.0f, &fontConfig, ImGui::GetIO().Fonts->GetGlyphRangesKorean());

    ImGui_ImplWin32_Init(this->GetHWND());
    ImGui_ImplDX11_Init(this->m_device.Get(), this->m_deviceContext.Get());

    Editor.OnGuiStart();
}

void WindowApp::UninitImgui()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void WindowApp::ImguiBeginDraw()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void WindowApp::ImguiEndDraw()
{

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
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

    ComPtr<ID3D11Texture2D> pBackBufferTexture;
    m_SwapChain1->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackBufferTexture); //스왑체인 백버퍼를 가져온다.
    if (pBackBufferTexture)
    {
        m_device->CreateRenderTargetView(pBackBufferTexture.Get(), nullptr, &m_backBufferRTV);

        D3D11_VIEWPORT viewPort{};
        viewPort.Width = clientSize.cx;
        viewPort.Height = clientSize.cy;
        m_deviceContext->RSSetViewports(1, &viewPort);
    }
}

void WindowApp::UninitDXGI()
{
   
}
