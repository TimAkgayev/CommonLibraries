#include <WindowsApplication.h>
#include <strsafe.h> //for error formating

int WindowsApplication::RunLoop()
{
	MSG msg = { 0 };
	mTimer.reset();

	Initialize();

	while (msg.message != WM_QUIT)
	{

		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		else
		{
			mTimer.tick();

			if (mTimer.getDeltaTime() < 0.016666f)
				Sleep(1);

			if (!mAppPaused)
				Update(mTimer.getDeltaTime());
			else
				Sleep(50);

			Draw();
		}
	}
	return (int)msg.wParam;

}


LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static WindowsApplication* app = 0;

	switch (msg)
	{
	case WM_CREATE:
	{
		// Get the 'this' pointer we passed to CreateWindow via the lpParam parameter.
		CREATESTRUCT* cs = (CREATESTRUCT*)lParam;
		app = (WindowsApplication*)cs->lpCreateParams;
		return 0;
	}
	}

	// Don't start processing messages until after WM_CREATE.
	if (app)
		return app->MessageProc(msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}




WindowsApplication::WindowsApplication() :mAppPaused(false)
{
}

WindowsApplication::~WindowsApplication()
{

}


HWND WindowsApplication::CreateAppWindow(HINSTANCE hInstance, UINT inWidth, UINT inHeight, const std::wstring& caption)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MainWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = "WindowsAppWndClass";


	if (!RegisterClass(&wc))
	{
		MessageBoxW(0, L"Failed to register the window class!", 0, 0);
		PostQuitMessage(0);
	}

	// Compute window rectangle dimensions based on requested client area dimensions.
	RECT R = { 0, 0, (LONG)inWidth, (LONG)inHeight };
	AdjustWindowRect(&R, WS_OVERLAPPEDWINDOW, false);
	mClientWidth = R.right - R.left;
	mClientHeight = R.bottom - R.top;




	mMainWindow = CreateWindowW(L"WindowsAppWndClass",
		caption.c_str(),
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		mClientWidth, mClientHeight,
		NULL, NULL,
		hInstance,
		this);
	if (!mMainWindow)
	{
		LPVOID lpMsgBuf;
		LPVOID lpDisplayBuf;
		DWORD dw = GetLastError();

		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER |
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			dw,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf,
			0, NULL);

		// Display the error message and exit the process

		lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
			(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)"CreateWindow") + 40) * sizeof(TCHAR));
		StringCchPrintf((LPTSTR)lpDisplayBuf,
			LocalSize(lpDisplayBuf) / sizeof(TCHAR),
			TEXT("%s failed with error %d: %s"),
			"CreateWindwo", dw, lpMsgBuf);
		MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);
		PostQuitMessage(0);
	}

	ShowWindow(mMainWindow, SW_SHOW);
	UpdateWindow(mMainWindow);

	return mMainWindow;
}

