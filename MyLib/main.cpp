#include <windows.h>
#include <memory>
#include <assert.h>
#include <tchar.h>

#include "framework.h"
//#include "GamePad.h"
#include "input_device.h"


LRESULT CALLBACK fnWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	framework *f = reinterpret_cast<framework*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
	return f ? f->handle_message(hwnd, msg, wparam, lparam) : DefWindowProc(hwnd, msg, wparam, lparam);
}

INT WINAPI wWinMain(HINSTANCE instance, HINSTANCE prev_instance, LPWSTR cmd_line, INT cmd_show)
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(72827);//72827
#endif
	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = fnWndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = instance;
	wcex.hIcon = 0;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = _T("クラッシュストーンズ");
	wcex.hIconSm = 0;
	RegisterClassEx(&wcex);

	RECT rc = { 0, 0, framework::SCREEN_WIDTH, framework::SCREEN_HEIGHT };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	HWND hwnd = CreateWindow(_T("クラッシュストーンズ"), _T("クラッシュストーンズ"), WS_OVERLAPPEDWINDOW ^ WS_MAXIMIZEBOX ^ WS_THICKFRAME | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, instance, NULL);
	ShowWindow(hwnd, cmd_show);

	SetClassLongW(hwnd, GCL_HICON, (LONG_PTR)LoadIconW(instance, (101 == 0) ? (LPWSTR)IDI_APPLICATION : MAKEINTRESOURCEW(101)));

	// TODO 追加2 : hwndとinstanceを取得。ウィンドウ情報が無いと初期化ができない(TODO 追加3へ)
	//dInput.hwnd = hwnd;
	//dInput.instance = instance;

	PAD.hwnd = hwnd;
	PAD.instance = instance;

	PAD.InitInputDevice();

	framework f(hwnd);
	SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(&f));

	SetMenu(hwnd, NULL);	//メニューを隠す
	SetWindowLong(hwnd, GWL_STYLE, WS_VISIBLE | WS_POPUP);//ウィンドウのスタイルを変更
	MoveWindow(hwnd, GetSystemMetrics(SM_XVIRTUALSCREEN),
		GetSystemMetrics(SM_YVIRTUALSCREEN),
		GetSystemMetrics(SM_CXVIRTUALSCREEN),
		GetSystemMetrics(SM_CYVIRTUALSCREEN), TRUE);

	return f.run(instance);
}