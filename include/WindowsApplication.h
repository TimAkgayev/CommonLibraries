#pragma once
#include <Windows.h>
#include <string>
#include "Timer.h"


class WindowsApplication
{
public:
	WindowsApplication();
	WindowsApplication(const WindowsApplication& rhs) = delete;
	WindowsApplication& operator=(const WindowsApplication& rhs) = delete;
	virtual ~WindowsApplication();

	virtual void Initialize() = 0;
	virtual void Update(float dt) = 0;
	virtual void Draw() = 0;
	virtual LRESULT MessageProc(UINT msg, WPARAM wParam, LPARAM lParam) = 0;


	int RunLoop();
	HWND CreateAppWindow(HINSTANCE hInstance, UINT width, UINT height, const std::wstring& caption);

protected:

	bool  mAppPaused;
	Timer mTimer;

protected:
	HWND mMainWindow;
	UINT mClientWidth;
	UINT mClientHeight;
};
