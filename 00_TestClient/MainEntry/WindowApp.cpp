#include "pch.h"
#include "WindowApp.h"
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
    Utility::RunBatchFile(batchPath);

}

void WindowApp::ModuleInitialize()
{
    InitD311();
    InitImgui();
    InitDXGI();

    ReloadDLLTest();
}


void WindowApp::PreUnitialize()
{

}

void WindowApp::ModuleUnitialize()
{
    testComponent.reset();
    if (m_scriptsDll != NULL)
    {
        FreeLibrary(m_scriptsDll);
    }    
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
    ImguiBeginDraw();
    {
        ImGui::Begin((const char*)u8"타임 클래스 확인용");
        {
            ImGui::InputDouble("time scale", &Time.timeScale);

            ImGui::Text("time : %f", Time.time());
            ImGui::Text("realtimeSinceStartup : %f", Time.realtimeSinceStartup());

            ImGui::Text("frameCount : %llu", Time.frameCount());

            ImGui::Text("FPS : %d", Time.frameRate());
            ImGui::Text("DeltaTime : %f", Time.deltaTime());

            ImGui::Text("unscaledDeltaTime : %f", Time.unscaledDeltaTime());

            ImGui::InputDouble("Fixed Time Step", &Time.fixedTimeStep);
            ImGui::Text("fixedDeltaTime %f", Time.fixedDeltaTime());
            ImGui::Text("fixedUnscaledDeltaTime %f", Time.fixedUnscaledDeltaTime());

            ImGui::InputDouble("maximumDeltaTime", &Time.maximumDeltaTime);

        }
        ImGui::End();
        
        ImGui::Begin((const char*)u8"도킹 확인용");
        {
            //프로퍼티 테스트
            ImGui::Private::InputAuto(Float);

            //리플렉션 테스트
            ImGui::InputReflectFields(this);   
        }
        ImGui::End();

        ImGui::Begin((const char*)u8"dll 테스트 용임");
        {
            if(testComponent)
                testComponent->Update(); //잘됌

            if(ImGui::Button((const char*)u8"DLL 리로드"))
            {
                ReloadDLLTest();
            }
        }
        ImGui::End();

        ImGui::Begin((const char*)u8"dll 리플렉션 테스트 용임");
        {
            if (testComponent)
                testComponent->imgui_draw_reflect_fields(); //잘됌
        }
        ImGui::End();
    }
    ImguiEndDraw();
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

    ImFontConfig fontConfig{};
    io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 20.0f, &fontConfig, ImGui::GetIO().Fonts->GetGlyphRangesKorean());

    ImGui_ImplWin32_Init(this->GetHWND());
    ImGui_ImplDX11_Init(this->m_device.Get(), this->m_deviceContext.Get());
}

void WindowApp::UninitImgui()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void WindowApp::ImguiBeginDraw()
{
    m_deviceContext->OMSetRenderTargets(1, m_backBufferRTV.GetAddressOf(), nullptr);
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

void WindowApp::ReloadDLLTest()
{
    using InitScripts = void(*)(const EngineCores& core);
    using NewScripts = Component *(*)();

    if (m_scriptsDll != NULL)
    {
        testComponent.reset();
        FreeLibrary(m_scriptsDll);
        m_scriptsDll = NULL;

        if (!Utility::RunBatchFile(batchPath))
            return;
    }

    if (!std::filesystem::exists(scriptsPath))
    {
        if (!Utility::RunBatchFile(batchPath))
            return;
    }

    m_scriptsDll = LoadLibraryW(scriptsPath);
    if (m_scriptsDll != NULL)
    {
        auto funcList = Utility::GetDLLFuntionNameList(m_scriptsDll);
        auto InitDLLCores = (InitScripts)GetProcAddress(m_scriptsDll, funcList[0].c_str());
        InitDLLCores(EngineCores{
            Time,
            sceneManager
            });

        auto NewTestComponent = (NewScripts)GetProcAddress(m_scriptsDll, funcList[1].c_str());
        Component* test = NewTestComponent();
        testComponent.reset(test);
    }
}

