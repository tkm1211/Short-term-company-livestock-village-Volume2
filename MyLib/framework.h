#pragma once

#include <windows.h>
#include <tchar.h>
#include <sstream>
#include <d3d11.h>

#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <imgui_internal.h>

#include "misc.h"
#include "high_resolution_timer.h"
#include"sprite.h"
#include"Blender.h"
#include"GeometricPrimitive.h"
#include"static_mesh.h"
#include"Skinned_mesh.h"
#include"Polygon.h"

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wparam, LPARAM lparam);

class framework
{

//	void SetBlendMode(Blender::BLEND_MODE blend)
//	{
//		deviceContext->OMSetBlendState(blender.modes[blend], nullptr, 0xFFFFFFFF);
//	}

public:
    CONST HWND hwnd = NULL;
	static CONST LONG SCREEN_WIDTH = 1920;
	static CONST LONG SCREEN_HEIGHT = 1080;
    /*-------------------------------------------------------------------*/
    /*TODO4 : frameworkクラスのメンバ変数として以下のCOMオブジェクトを宣言する*/
    /*-------------------------------------------------------------------*/

    //デバイスインターフェイスは仮想アダプタを表す。リソースを作成するために使用されます。
    static ID3D11Device*           device;
    //コマンドをレンダリング生成デバイスコンテキストを表す。
	static ID3D11DeviceContext*    deviceContext;
    //レンダリングされたデータを出力に表示する前に格納するための1つまたは複数のサーフェスを実装します。
    IDXGISwapChain*         swapChain = nullptr;
    //レンダリング中にアクセスできるレンダーターゲットサブリソースを識別する。
    ID3D11RenderTargetView* renderTaegetView = nullptr;
    //深度ステンシルテスト中にテクスチャリソースにアクセスする。
    ID3D11DepthStencilView* depthStencilView = nullptr;


    D3D_DRIVER_TYPE         driverType = D3D_DRIVER_TYPE_NULL;

	//Spriteクラスのポインタ
	Sprite* sprites[1024];
	//geometric_primitiveのポインタ
	geometric_primitive* cube;

	//Benchimarkのテスト用実体
	Sprite* particle;
	Sprite* font;

	//SpriteBatch用ポインタ
	SpriteBatch* batch;

	//BoxPolygon用のポインタ
	Box*	box;

	//StaticMeshクラスのポインタ
	StaticMesh* mesh;

	//SkinnedMeshクラスのポインタ
	SkinnedMesh* skinMesh;

	int id;

	BOOL isMause;

	//Blender* b;

	framework()
	{
	}
	framework(HWND hwnd) : hwnd(hwnd)
	{
	}
	~framework()
	{
		if (depthStencilView)
			depthStencilView->Release();
		if (renderTaegetView)
			renderTaegetView->Release();
		if (swapChain)
			swapChain->Release();
		if (deviceContext)
			deviceContext->Release();
		if (device)
			device->Release();
		if (cube)
			delete cube;
		if (particle)
			delete particle;
		if (font)
			delete font;
		if (batch)
			delete batch;
		for (auto& data : sprites)
		{
			if (data)
				delete data;
			data = nullptr;
		}
		cube = nullptr;
		particle = nullptr;
		font = nullptr;
		mesh = nullptr;

	}
	int run(HINSTANCE _hInstance);

	LRESULT CALLBACK handle_message(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		{
			return 1;
		}

		switch (msg)
		{
		case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc;
			hdc = BeginPaint(hwnd, &ps);
			EndPaint(hwnd, &ps);
			break;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_CREATE:
			break;
		case WM_KEYDOWN:
			if (wparam == VK_ESCAPE) PostMessage(hwnd, WM_CLOSE, 0, 0);
			break;
		case WM_ENTERSIZEMOVE:
			// WM_EXITSIZEMOVE is sent when the user grabs the resize bars.
			timer.stop();
			break;
		case WM_EXITSIZEMOVE:
			// WM_EXITSIZEMOVE is sent when the user releases the resize bars.
			// Here we reset everything based on the new window dimensions.
			timer.start();
			break;
		default:
			return DefWindowProc(hwnd, msg, wparam, lparam);
		}
		return 0;
	}

private:
	bool initialize(HINSTANCE _hInstance);
	void update(float elapsed_time/*Elapsed seconds from last frame*/);
	void render(float elapsed_time/*Elapsed seconds from last frame*/);

private:
	high_resolution_timer timer;
	void calculate_frame_stats()
	{
		// Code computes the average frames per second, and also the 
		// average time it takes to render one frame.  These stats 
		// are appended to the window caption bar.
		static int frames = 0;
		static float time_tlapsed = 0.0f;

		frames++;

		// Compute averages over one second period.
		if ((timer.time_stamp() - time_tlapsed) >= 1.0f)
		{
			float fps = static_cast<float>(frames); // fps = frameCnt / 1
			float mspf = 1000.0f / fps;
			std::ostringstream outs;
			outs.precision(6);
			outs << "FPS : " << fps << " / " << "Frame Time : " << mspf << " (ms)";
			SetWindowTextA(hwnd, outs.str().c_str());

			// Reset for next average.
			frames = 0;
			time_tlapsed += 1.0f;
		}
	}

public:
	void	PopInstance();
	void	RenderSprite();
	void	RenderMesh(DirectX::XMFLOAT4X4, DirectX::XMFLOAT4X4, DirectX::XMFLOAT4, DirectX::XMFLOAT4);

	static framework* GetInstance()
	{
		static framework instance;
		return &instance;
	}

	ID3D11Device*			GetDevice()			{ return device; }
	ID3D11DeviceContext*	GetDeviceContext()	{ return deviceContext; }
	HWND					GetHWND()			{ return hwnd; }
};

#define FRAMEWORK	framework::GetInstance()
