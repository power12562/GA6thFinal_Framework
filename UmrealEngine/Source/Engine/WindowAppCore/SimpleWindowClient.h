#pragma once
#define WIN32_LEAN_AND_MEAN

#pragma comment(lib, "Dbghelp.lib")

struct ClientHelper
{
	/**클라이언트를 화면 가운데로 위치 시킨다.*/
	static void WinToScreenCenter(HWND hwnd);

	/**클라이언트 크기를 현재 화면 해상도 설정으로 Clamp 하는 함수.*/
	static void ClampScreenMaxSize(SIZE& size);
};

class SimpleWindowClient
{
	friend LRESULT CALLBACK DefaultWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
public:
	SimpleWindowClient() = default;
	virtual ~SimpleWindowClient() = default;

	void Initialize(HINSTANCE hinstance);
	void Run();
	void Uninitialize();

	/** 현재 실행중인 게임루프를 종료시킵니다.*/
	void RunEnd();

	HINSTANCE GetHInstance() const { return hInstance; }
	HWND	  GetHWND() const { return hwnd; }
protected:
	/*1번째 호출되는 초기화 함수*/
	virtual void PreInitialize()	= 0;
	/*2번째 호출되는 초기화 함수*/
	virtual void ModuleInitialize() = 0;

	/*1번째 호출되는 정리 함수*/
	virtual void ModuleUnitialize() = 0; 
	/*2번째 호출되는 정리 함수*/
	virtual void PreUnitialize()	= 0;

	virtual void ClientUpdate()		= 0;
	virtual void ClientRender()		= 0;

protected:
	/** 클래스 스타일. Initialize() 실행전에만 적용됩니다.*/
	UINT winClassStyle = CS_HREDRAW | CS_VREDRAW;

	/** 클라이언트 윈도우 스타일. Initialize() 실행전에만 적용됩니다. 기본값 : 테두리 없는 창모드*/
	DWORD windowStyleEX = WS_POPUP;

	/** 윈도우 클래스 구조체 이름. Initialize() 실행전에만 적용됩니다.*/
	LPCWSTR winClassName = L"UmrealEngineClass";

	/** 윈도우 클라이언트 이름. Initialize() 실행전에만 적용됩니다.*/
	LPCWSTR windowName = L"DemoApp";

	/** 사용할 윈도우 프로시저 (nullptr이면 기본 프로시저를 사용합니다.) Initialize() 실행전에만 적용됩니다.*/
	WNDPROC customWndProc = nullptr;

	/** 클라이언트 사이즈. Initialize() 실행전에만 적용됩니다. 기본 값 : 화면 해상도*/
	SIZE clientSize = { 0, 0 };

protected:
	/*윈도우 스타일을 창모드로*/
	void SetStyleToWindowed();
	/*윈도우 스타일을 테두리 없는 창모드로*/
	void SetStyleToBorderlessWindowed();

	/*클라이언트 크기를 모니터 해상도로*/
	void SetOptimalScreenSize();

protected:
	bool isRunEnd = false;
private:
	MSG msg{};

private:
	inline static HINSTANCE hInstance{};
	HWND hwnd{};
	void WindowInit(HINSTANCE _hinstance);
	void WindowUninit();
};