#include "pch.h"
#include "Game.h"

WCHAR			g_WindowClass[MAX_NAME_STRING];
WCHAR			g_WindowTitle[MAX_NAME_STRING];
INT				g_WindowWidth;
INT				g_WindowHeight;
HICON			g_HIcon;
HWND			g_HWnd;
Game			g_Game;

VOID InitializeVariables();
VOID CreateWindowClass();
VOID InitializeWindow();
VOID MessageLoop();

LRESULT CALLBACK WindowProcess(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam);

INT CALLBACK WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	InitializeVariables();

	CreateWindowClass();

	InitializeWindow();

	g_Game.Init(g_HWnd);

	MessageLoop();

	return 0;
}

LRESULT CALLBACK WindowProcess(HWND hWnd, UINT message, WPARAM wparam, LPARAM lparam)
{
	switch (message) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, message, wparam, lparam);
}

VOID InitializeVariables() {
	wcscpy_s(g_WindowClass, TEXT("DirectXProject"));
	wcscpy_s(g_WindowTitle, TEXT("DirectX window"));
	g_WindowWidth = g_WinSizeX;
	g_WindowHeight = g_WinSizeY;
	g_HIcon = LoadIcon(HInstance(), MAKEINTRESOURCE(IDI_MAINICON));
}

VOID CreateWindowClass()
{
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
	wcex.hIcon = g_HIcon;
	wcex.hIconSm = g_HIcon;
	wcex.lpszClassName = g_WindowClass;
	wcex.lpszMenuName = nullptr;
	wcex.hInstance = HInstance();
	wcex.lpfnWndProc = WindowProcess;
	RegisterClassEx(&wcex);
}

VOID InitializeWindow() {
	g_HWnd = CreateWindow(g_WindowClass, g_WindowTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, g_WindowWidth, g_WindowHeight, nullptr, nullptr, HInstance(), nullptr);
	if (!g_HWnd) {
		MessageBox(0, L"Failed to Create Window!.", 0, 0);
		PostQuitMessage(0);
	}

	ShowWindow(g_HWnd, SW_SHOW);
}

VOID MessageLoop() {
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			g_Game.Update();
			g_Game.Render();
		}
	}
}