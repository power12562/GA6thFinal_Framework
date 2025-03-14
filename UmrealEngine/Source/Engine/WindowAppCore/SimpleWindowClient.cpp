#include "pch.h"
#include "SimpleWindowClient.h"

static LRESULT DefaultWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

void ClientHelper::WinToScreenCenter(HWND hwnd)
{
	int x, y, width, height;
	RECT rtDesk, rtWindow;
	GetWindowRect(GetDesktopWindow(), &rtDesk);
	GetWindowRect(hwnd, &rtWindow);
	width = rtWindow.right - rtWindow.left;
	height = rtWindow.bottom - rtWindow.top;
	x = (rtDesk.right - width) / 2;
	y = (rtDesk.bottom - height) / 2;
	MoveWindow(hwnd, x, y, width, height, FALSE);
}

void ClientHelper::ClampScreenMaxSize(SIZE& size)
{
	const SIZE maxScreenSize = { GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN) };
	if (size.cx <= 0 || 0 >= size.cy ||
		size.cx > maxScreenSize.cx || maxScreenSize.cy < size.cy)
	{
		size = maxScreenSize;
	}
}

void SimpleWindowClient::Initialize(HINSTANCE _hinstance)
{
	SetUnhandledExceptionFilter(CustomUnhandledExceptionFilter);
	WindowInit(_hinstance);
	PreInitialize();
	ModuleInitialize();
}

void SimpleWindowClient::Run()
{
	// PeekMessage 메세지가 있으면 true,없으면 false
	while (!isRunEnd)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg); // 키입력관련 메시지 변환  WM_KEYDOWN -> WM_CHAR
			DispatchMessage(&msg);
		}
		else
		{
			ClientUpdate();
			ClientRender();
		}
	}
}

void SimpleWindowClient::Uninitialize()
{
	ModuleUnitialize();
	PreUnitialize();
	WindowUninit();
}

void SimpleWindowClient::RunEnd()
{
	isRunEnd = true;
}

void SimpleWindowClient::WindowInit(HINSTANCE _hinstance)
{
	SimpleWindowClient::hInstance = _hinstance;

	// 윈도우 클래스 구조체 초기화
	WNDCLASSEX wc = { 0 };
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = winClassStyle;
	if (customWndProc != nullptr)
	{
		wc.lpfnWndProc = customWndProc;
	}
	else
	{
		wc.lpfnWndProc = DefaultWndProc;	// 기본 윈도우 프로시저 함수
	}
	wc.hInstance = hInstance;   // 인스턴스 핸들
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = winClassName;

	// 윈도우 클래스 등록
	if (!RegisterClassEx(&wc))
	{
		int error = GetLastError();
		wchar_t errorStr[25]{};
		swprintf_s(errorStr, L"윈도우 클래스 등록 실패. 에러 : %d", error);

		MessageBox(NULL, errorStr, L"에러", MB_OK | MB_ICONERROR);
		isRunEnd = true;
		return;
	}
	
	ClientHelper::ClampScreenMaxSize(clientSize);
	RECT clientRect = { 0, 0, clientSize.cx, clientSize.cy };
	AdjustWindowRect(&clientRect, windowStyleEX, FALSE);
	SIZE windowSize{ clientRect.right - clientRect.left , clientRect.bottom - clientRect.top };
	SIZE windowClientOffset{ windowSize.cx - clientSize.cx, windowSize.cy - clientSize.cy };

	clientSize.cx = windowSize.cx - windowClientOffset.cx;
	clientSize.cy = windowSize.cy - windowClientOffset.cy;

	// 윈도우 생성
	hwnd = CreateWindowEx(
		0,
		winClassName,
		windowName,
		windowStyleEX,
		0, 0,
		windowSize.cx, windowSize.cy,
		NULL, NULL, hInstance, NULL
	);

	if (!hwnd)
	{
		int error = GetLastError();
		wchar_t errorStr[25]{};

		swprintf_s(errorStr, L"윈도우 생성 실패. 에러 : %d", error);
		MessageBox(hwnd, errorStr, L"에러", MB_OK | MB_ICONERROR);
		isRunEnd = true;
		return;
	}

	// 윈도우 표시
	ShowWindow(hwnd, 10);
	UpdateWindow(hwnd);
	ClientHelper::WinToScreenCenter(hwnd);
}

void SimpleWindowClient::WindowUninit()
{


}

static LRESULT DefaultWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
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

void SimpleWindowClient::SetStyleToWindowed()
{
	windowStyleEX = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;
    //windowStyleEX = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
}

void SimpleWindowClient::SetStyleToBorderlessWindowed()
{
	windowStyleEX = WS_POPUP;
}

void SimpleWindowClient::SetOptimalScreenSize()
{
	clientSize = { 0, 0 };
}