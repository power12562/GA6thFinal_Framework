#include "pch.h"
#include "WindowApp.h"

#include "blueprint.h"
blueprint nodeEditor;

int APIENTRY wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow
)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    WindowApp app(hInstance);
    app.Initialize();
    app.Run();
    app.UnInitialize();

    return 0;
}

static void DrawTransformNodeTest(const Transform& node)
{
    ImGui::PushID(node.gameObject.GetInstanceID());
    if (ImGui::TreeNode(node.gameObject.ToString().c_str()))
    {
        for (int i = 0; i < node.childCount; i++)
        {
            Transform& child = *node.GetChild(i);
            DrawTransformNodeTest(child); // 재귀 호출
        }
        ImGui::TreePop();
    }
    ImGui::PopID();
};

WindowApp::WindowApp(HINSTANCE hInstance)
    : Application(hInstance)
{
    WindowApp::D311Module::App = this;
    this->AddModule<WindowApp::D311Module>();

    WindowApp::ImguiModule::App = this;
    this->AddModule<WindowApp::ImguiModule>();

    WindowApp::DXGIModule::App = this;
    this->AddModule<WindowApp::DXGIModule>();

}
WindowApp::~WindowApp() = default;

void WindowApp::ClientUpdate()
{
    constexpr float clearColor[] = { 0.f,0.f,0.f,1.f };
    m_deviceContext->ClearRenderTargetView(m_backBufferRTV.Get(), clearColor);
    ImguiBeginDraw();
    ESceneManager::Engine::SceneUpdate();
}

void WindowApp::ClientRender()
{
    using namespace u8_literals;
    {
        ImGui::Begin(u8"타임 클래스 확인용"_c_str);
        {
            ImGui::InputDouble("time scale", &EngineCore->Time.timeScale);

            ImGui::Text("time : %f", EngineCore->Time.time());
            ImGui::Text("realtimeSinceStartup : %f", EngineCore->Time.realtimeSinceStartup());

            ImGui::Text("frameCount : %llu", EngineCore->Time.frameCount());

            ImGui::Text("FPS : %d", EngineCore->Time.frameRate());
            ImGui::Text("DeltaTime : %f", EngineCore->Time.deltaTime());

            ImGui::Text("unscaledDeltaTime : %f", EngineCore->Time.unscaledDeltaTime());

            ImGui::InputDouble("Fixed Time Step", &EngineCore->Time.fixedTimeStep);
            ImGui::Text("fixedDeltaTime %f", EngineCore->Time.fixedDeltaTime());
            ImGui::Text("fixedUnscaledDeltaTime %f", EngineCore->Time.fixedUnscaledDeltaTime());

            ImGui::InputDouble("maximumDeltaTime", &EngineCore->Time.maximumDeltaTime);
        }
        ImGui::End();

        std::shared_ptr<GameObject> m_testObject = GameObject::Find(L"TestObject").lock();
        ImGui::Begin(u8"컴포넌트 추가 테스트"_c_str);
        {
            if (m_testObject)
            {
                for (size_t i = 0; i < m_testObject->GetComponentCount(); i++)
                {
                    std::weak_ptr<Component> wptr = m_testObject->GetComponentAtIndex<Component>(i);
                    if (auto component = wptr.lock())
                    {
                        ImGui::PushID(i);
                        if (ImGui::CollapsingHeader(component->class_name()))
                        {
                            component->imgui_draw_reflect_fields();
                        }
                        ImGui::PopID();
                        std::string json = component->serialized_reflect_fields();
                    }
                }

                if (ImGui::CollapsingHeader(u8"컴포넌트 추가하기"_c_str))
                {
                    for (auto& key : EngineCore->ComponentFactory.GetNewComponentFuncList())
                    {
                        if (ImGui::Button(key.c_str()))
                        {
                            if (m_testObject)
                            {
                                EngineCore->ComponentFactory.AddComponentToObject(m_testObject.get(), key);
                            }
                        }
                    }
                }
            }
        }
        ImGui::End();
        
        ImGui::Begin(u8"프로퍼티 테스트"_c_str);
        {
            if (m_testObject)
            {
                bool input = m_testObject->activeSelf;
                ImGui::Checkbox("object Active", &input);
                m_testObject->SetActive(input);

                for (int i = 0;  i < m_testObject->GetComponentCount(); i++)
                {
                    if (auto component = m_testObject->GetComponentAtIndex<Component>(i).lock()) 
                    {
                        bool input = component->Enable;
                        ImGui::Checkbox(component->class_name(), &input);
                        component->Enable = input;
                    }
                }
            }
        }
        ImGui::End();

        ImGui::Begin(u8"컴포넌트 팩토리 테스트"_c_str);
        {
            if (ImGui::Button(u8"스크립트 재 빌드"_c_str))
            {
                EngineCore->ComponentFactory.InitalizeComponentFactory();
            }

            //새 스크립트 파일 만들기 테스트용
            {
                static ImVec2 popupPos{};
                static std::string inputBuffer{};
                if (ImGui::Button(u8"테스트 스크립트 파일 만들기"_c_str))
                {
                    popupPos = ImGui::GetMousePos();
                    inputBuffer.clear();
                    ImGui::OpenPopup(u8"파일 이름을 입력하세요."_c_str);
                }
                
                if (ImGui::BeginPopup(u8"파일 이름을 입력하세요."_c_str))
                {
                    ImGui::SetNextWindowPos(popupPos, ImGuiCond_Appearing);
                    ImGui::InputText("##new_script_file_name", &inputBuffer);
                    if (ImGui::Button("OK"))
                    {
                        EngineCore->ComponentFactory.MakeScriptFile(inputBuffer.c_str());
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
    }

    ImGui::Begin("Node Editor");
    {
        nodeEditor.OnFrame(0.016f);
    }
    ImGui::End();

    ImGui::Begin(u8"트리 테스트"_c_str);
    {
        const auto& objList = EngineCore->SceneManager.GetRootObjects();
        for (auto& obj : objList)
        {
            if (auto ptr = obj.lock())
            {
                DrawTransformNodeTest(ptr->transform);
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

    ImGui_ImplWin32_Init(GetHwnd());
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
    swapDesc.Width = _clientSize.cx;
    swapDesc.Height = _clientSize.cy;
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

    m_DXGIFactory4->CreateSwapChainForHwnd(m_device.Get(), GetHwnd(), &swapDesc, &screenDesc, nullptr, &m_SwapChain1);

    ComPtr<ID3D11Texture2D> pBackBufferTexture;
    m_SwapChain1->GetBuffer(0, __uuidof(ID3D11Texture2D), &pBackBufferTexture); //스왑체인 백버퍼를 가져온다.
    if (pBackBufferTexture)
    {
        m_device->CreateRenderTargetView(pBackBufferTexture.Get(), nullptr, &m_backBufferRTV);

        D3D11_VIEWPORT viewPort{};
        viewPort.Width = _clientSize.cx;
        viewPort.Height = _clientSize.cy;
        m_deviceContext->RSSetViewports(1, &viewPort);
    }
}

void WindowApp::UninitDXGI()
{

}

void WindowApp::D311Module::ModuleInitialize()
{
    App->InitD311();
}

void WindowApp::D311Module::ModuleUnInitialize()
{
    App->UninitD311();
}

void WindowApp::ImguiModule::ModuleInitialize()
{
    App->InitImgui();
}

void WindowApp::ImguiModule::ModuleUnInitialize()
{
    App->UninitImgui();
}

void WindowApp::DXGIModule::ModuleInitialize()
{
    App->InitDXGI();

    //오브젝트 생성 테스트
    auto obj1 = NewGameObject<GameObject>(L"TestObject").lock();
    auto obj2 = NewGameObject<GameObject>(L"TestObject2").lock();
    auto obj3 = NewGameObject<GameObject>(L"TestObject3").lock();
    auto obj4 = NewGameObject<GameObject>(L"TestObject4").lock();
    auto obj5 = NewGameObject<GameObject>(L"TestObject5").lock();
    auto obj6 = NewGameObject<GameObject>(L"TestObject6").lock();

    //Scene Graph 테스트
    obj2->transform.SetParent(obj1->transform);
    obj3->transform.SetParent(obj2->transform);
    obj4->transform.SetParent(obj3->transform);
    obj5->transform.SetParent(obj4->transform);
    obj6->transform.SetParent(obj5->transform);

    nodeEditor.OnStart();
}

void WindowApp::DXGIModule::ModuleUnInitialize()
{
    App->UninitDXGI();
}
