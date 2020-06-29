#include "Precompiled.h"
#include "Window.h"

using namespace S;
using namespace S::Core;

LRESULT CALLBACK WinProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}
	return DefWindowProcA(handle, message, wParam, lParam);
}

Window::Window()
	: mInstance(nullptr)
	, mWindow(nullptr)
{}

Window::~Window()
{}

void Window::Initialize(HINSTANCE instance, LPCSTR appName, uint32_t width, uint32_t height)
{
	mInstance = instance;
	mAppName = appName;

	// Every Windows Window requires at least one window object. Three things are involved
	// 1. Register a window class
	// 2. Create a window object
	// 3. Retrieve and dispatch messages for this window.

	WNDCLASSEXA wcex;
	wcex.cbSize = sizeof(WNDCLASSEXA);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WinProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = appName;
	wcex.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	RegisterClassExA(&wcex);
	// Compute the correct window dimension
	RECT rc = { 0,0,(LONG)width,(LONG)height };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	mScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	mScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	const int winWidth = rc.right - rc.left;
	const int winHeight = rc.bottom - rc.top;
	const int left = (mScreenWidth - winWidth) / 2;
	const int top = (mScreenHeight - winHeight) / 2;

	mWindow = CreateWindowA
	(
		appName, appName,
		WS_OVERLAPPEDWINDOW,
		left, top,
		winWidth, winHeight,
		nullptr, nullptr,
		instance, nullptr
	);

	ShowWindow(mWindow, true);

	SetCursorPos(mScreenWidth / 2, mScreenHeight / 2);
}

void Window::Terminate()
{
	DestroyWindow(mWindow);

	UnregisterClassA(mAppName.c_str(), mInstance);

	mWindow = nullptr;
	mInstance = nullptr;
}

const uint32_t S::Core::Window::GetScreenWidth() const
{
	return mScreenWidth;
}

const uint32_t S::Core::Window::GetScreenHeight() const
{
	return mScreenHeight;
}

bool Window::ProcessMessage()
{
	MSG msg = { 0 };
	bool quit = false;
	while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);

		if (WM_QUIT == msg.message)
		{
			quit = true;
		}
	}
	return quit;
}