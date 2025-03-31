#pragma once

/// <summary>
/// <para> 모듈들의 초기화 및 자원 정리를 자동화 하기 위한 인터페이스입니다.                  </para>
/// <para> 상속 받은뒤, 클라이언트의 Application Drived 클래스의 생성자에서 등록해주면 됩니다.</para>
/// <para> 초기화 순서는 모든 모듈의 PreInit -> ModuleInitialize 순입니다. </para>
/// <para> 해제 순서는 모든 모듈의 ModuleUnInitialize -> PerUnInitialize 순입니다.  </para>
/// <para> 우선순위가 중요하면 InitOrder로 제어합니다. 기본값 : 0</para>
/// </summary>
struct IAppModule
{
    IAppModule() = default;
    virtual ~IAppModule() = default;

    virtual void PreInitialize() = 0;
    virtual void ModuleInitialize() = 0;

    virtual void ModuleUnInitialize() = 0;
    virtual void PreUnInitialize() = 0;

    virtual long InitOrder() { return 0; };
};

/// <summary>
/// 메시지 이벤트를 위한 핸들러입니다.
/// </summary>
struct MessageHandler
{
    friend class Application;
    using Handle = std::function<bool(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)>;
    MessageHandler(const Handle& handle, long messageOrder) :
        _handle(handle),
        _messageOrder(messageOrder)
    {
        
    }
    MessageHandler (const MessageHandler& rhs) :
        _handle(rhs._handle),
        _messageOrder(rhs._messageOrder)
    {
     
    }
    bool operator<(const MessageHandler& rhs) const
    {
        return this->_messageOrder < rhs._messageOrder;
    }
private:
    Handle _handle;
    long _messageOrder;
};  

class Application
{
    inline static Application* App = nullptr;
    static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
    /// <summary>
    /// 앱을 종료합니다.
    /// </summary>
    inline static void Quit()
    {
        if (App)
        {
            App->_isQuit = true;
        }      
    }
    /// <summary>
    /// 현재 실행중인 클라이언트의 HINSTANCE를 반환합니다.
    /// </summary>
    /// <returns>HINSTANCE</returns>
    inline static HINSTANCE GetHInstance()
    {
        if (App)
        {
            return App->_hInstance;
        }
        return NULL;
    }
    /// <summary>
    /// 현재 실행중인 클라이언트의 HWND를 반환합니다.
    /// </summary>
    /// <returns>HWND</returns>
    inline static HWND GetHwnd()
    {
        if (App)
        {
            return App->_hWnd;
        }
        return NULL;
    }
    /// <summary>
    /// 메시지 핸들러를 등록합니다. Application에서 메시지 피크시 핸들러들에게 메시지를 전달해줍니다.
    /// </summary>
    /// <param name="handle :">사용할 핸들러 함수</param>
    /// <param name="messageOrder :">우선순위</param>
    inline static void AddMessageHandler(const MessageHandler& handle)
    {
        if (App == nullptr)
        {
            assert(!"Application이 생성되지 않았습니다.");
            return;
        }       
        App->_messageHandleList.emplace_back(handle);
        std::sort(App->_messageHandleList.begin(), App->_messageHandleList.end(), 
            [](MessageHandler& handleA, MessageHandler& HandleB)
            {
                return handleA < HandleB;
            });
    }

public:
    Application(HINSTANCE hInstance);
    virtual ~Application() = default;
    
    void Initialize();
    void UnInitialize();
    void Run();

protected:
    /// <summary>
    /// 모듈을 등록할때 사용합니다.
    /// Application을 상속받은 클래스의 생성자에서 호출해야지 정상적인 초기화가 이루어집니다.
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template <typename T> void AddModule()
    {
        //모듈 상속받은 클래스만 등록 가능.
        static_assert(std::is_base_of_v<IAppModule, T>, "T is not a module.");
        _appModuleList.emplace_back(new T);
        std::sort(_appModuleList.begin(), _appModuleList.end(), [](std::unique_ptr<IAppModule>& appModuleA, std::unique_ptr<IAppModule>& appModuleB)
            {
                return appModuleA->InitOrder() < appModuleB->InitOrder();
            });
    }
    /*윈도우 스타일을 창모드로*/
    void SetStyleToWindowed();
    /*윈도우 스타일을 테두리 없는 창모드로*/
    void SetStyleToBorderlessWindowed();
    /*클라이언트 크기를 모니터 해상도로*/
    void SetOptimalScreenSize();

protected:
    /// <summary>
    /// 클라이언트의 업데이트 부분
    /// </summary>
    virtual void ClientUpdate() = 0;
    /// <summary>
    /// 클라이언트의 렌더 부분
    /// </summary>
    virtual void ClientRender() = 0;

protected:
    /** 클래스 스타일. Initialize() 실행전에만 적용됩니다.*/
    UINT _winClassStyle = CS_HREDRAW | CS_VREDRAW;

    /** 클라이언트 윈도우 스타일. Initialize() 실행전에만 적용됩니다. 기본값 : 테두리 없는 창모드*/
    DWORD _windowStyleEX = WS_POPUP;

    /** 윈도우 클래스 구조체 이름. Initialize() 실행전에만 적용됩니다.*/
    LPCWSTR _winClassName = L"UmrealEngineClass";

    /** 윈도우 클라이언트 이름. Initialize() 실행전에만 적용됩니다.*/
    LPCWSTR _windowName = L"DemoApp";

    /** 클라이언트 사이즈. Initialize() 실행전에만 적용됩니다. 기본 값 : 화면 해상도*/
    SIZE _clientSize = { 0, 0 };
private:
    bool _isQuit = false;
    HWND _hWnd = NULL;
    HINSTANCE _hInstance = NULL;
    MSG _msg{};

    std::vector<std::unique_ptr<IAppModule>> _appModuleList;
    std::vector<MessageHandler> _messageHandleList;
};

struct ClientHelper
{
    /**클라이언트를 화면 가운데로 위치 시킵니다.*/
    static void WinToScreenCenter(HWND hwnd);

    /**클라이언트 크기를 현재 화면 해상도 설정으로 Clamp 하는 함수.*/
    static void ClampScreenMaxSize(SIZE& size);
};
