﻿#include "pch.h"
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
    m_testObject = std::make_unique<GameObject>();
}

void WindowApp::ModuleInitialize()
{
    InitD311();
    InitImgui();
    InitDXGI();
    componentFactory.InitalizeComponentFactory();
}


void WindowApp::PreUnitialize()
{


}

void WindowApp::ModuleUnitialize()
{
    componentFactory.UninitalizeComponentFactory();
    UninitDXGI();
    UninitImgui();
    UninitD311();
}

void WindowApp::ClientUpdate()
{
    SceneManager::Engine::SceneUpdate();
}

void WindowApp::ClientRender()
{
    using namespace u8_literals;

    constexpr float clearColor[] = { 0.f,0.f,0.f,1.f };
    m_deviceContext->ClearRenderTargetView(m_backBufferRTV.Get(), clearColor);

    ImguiBeginDraw();
    {
        ImGui::Begin(u8"타임 클래스 확인용"c_str);
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
        
        ImGui::Begin(u8"컴포넌트 추가 테스트"c_str);
        {
            for (size_t i = 0; i < m_testObject->GetComponentCount(); i++)
            {
                std::weak_ptr<Component> wptr = m_testObject->GetComponentAtIndex<Component>(i);
                if (auto component = wptr.lock())
                {
                    ImGui::PushID(i);
                    if(ImGui::CollapsingHeader(component->class_name()))
                    {
                        component->imgui_draw_reflect_fields();
                    }                   
                    ImGui::PopID();
                    component->Update(); //dll 업데이트 테스트
                    std::string json = component->serialized_reflect_fields();
                }
            }

            if (ImGui::CollapsingHeader(u8"컴포넌트 추가하기"c_str))
            {
                for (auto& key : componentFactory.GetNewComponentFuncList())
                {
                    if (ImGui::Button(key.c_str()))
                    {
                        if (m_testObject)
                        {
                            componentFactory.AddComponentToObject(m_testObject.get(), key);
                        }
                    }
                }
            }
        }
        ImGui::End();
    }
    ImGui::Begin(u8"프로퍼티 테스트"c_str);
    {

    }
    ImGui::End();

    ImGui::Begin(u8"컴포넌트 팩토리 테스트"c_str);
    {
        if (ImGui::Button(u8"스크립트 재 빌드"c_str))
        {
            componentFactory.InitalizeComponentFactory();
        }

        //새 스크립트 파일 만들기 테스트용
        {
            static ImVec2 popupPos{};
            static std::string inputBuffer{};
            if (ImGui::Button(u8"테스트 스크립트 파일 만들기"c_str))
            {
                popupPos = ImGui::GetMousePos();
                inputBuffer.clear();
                ImGui::OpenPopup(u8"파일 이름을 입력하세요."c_str);
            }
            
            if (ImGui::BeginPopup(u8"파일 이름을 입력하세요."c_str))
            {
                ImGui::SetNextWindowPos(popupPos, ImGuiCond_Appearing);
                ImGui::InputText("##new_script_file_name", &inputBuffer);
                if (ImGui::Button("OK"))
                {
                    componentFactory.MakeScriptFile(inputBuffer.c_str());
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }
        }
    }
    ImGui::End();

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

