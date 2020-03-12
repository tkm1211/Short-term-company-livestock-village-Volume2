#include <wrl.h>
#include <deque>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include "Audio.h"
#include "framework.h"
#include "sprite.h"
#include "Camera.h"
#include <SceneManager.h>
#include <input_device.h>

/*---------------------------------------------------------------*/
/*TODO6 : frameworkクラスのinitializeメンバ関数で以下の処理を実装する*/
/*---------------------------------------------------------------*/
    //1. 描画先の縦横サイズ、フォーマット(形式)を設定
    //
    //2. 描画先の更新頻度(60fpsだったり30fpsだったりというあれ)を分数形式で設定
    //
    //3. 他、描画先の使用方法(まあここではDXGI_USAGE_RENDER_TARGET_OUTPUTだろう)、描画先ウィンドウのハンドル、ウィンドウモードか否か、MSAAのクオリティやカウント等を設定
    //
    //4. ここまでがSwapChainのパラメータ設定。次にDevice、Contextの能力レベルを設定する。D3D_FEATURE_LEVEL_11_0でDirectX 11の基準を完全に満たしたGPU上で動作するレベルとなる。もし足りない場合は10_1 -> 10_0 -> 9_3 -> 9_2 -> 9_1と、徐々に下げていく等を行い、成功することを祈る。

//static変数の宣言
 ID3D11Device* framework::device = nullptr;
 ID3D11DeviceContext* framework::deviceContext = nullptr;


 int framework::run(HINSTANCE _hInstance)
 {
	 MSG msg = {};

	 if (!initialize(_hInstance)) return 0;

	 ImGui::SetCurrentContext(ImGui::CreateContext());
	 ImGui_ImplWin32_Init(hwnd);
	 ImGui_ImplDX11_Init(device, deviceContext);
	 ImGui::StyleColorsDark();

	 ImGuiIO &io = ImGui::GetIO();
	 io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	 //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\meiryo.ttc", 18.0f, NULL, glyphRangesJapanese);
	 //io.Fonts->AddFontFromFileTTF(".\\consolab.ttf", 10.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	 //io.Fonts->AddFontFromFileTTF(".\\Inconsolata-Bold.ttf", 12.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	 while (WM_QUIT != msg.message)
	 {
		 if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		 {
			 TranslateMessage(&msg);
			 DispatchMessage(&msg);
		 }
		 else
		 {
			 timer.tick();
//			 calculate_frame_stats();
			 update(timer.time_interval());
			 render(timer.time_interval());
		 }
	 }
	 SCENE_MANAGER->Uninit();
	 ImGui_ImplDX11_Shutdown();
	 ImGui_ImplWin32_Shutdown();
	 ImGui::DestroyContext();

	 return static_cast<int>(msg.wParam);
 }


bool framework::initialize(HINSTANCE _hInstance)
{
    //HRESULT = 0(true)=成功、0x00000000～oxffffffff(false)=失敗
    HRESULT hr = S_OK;
    //RECT = LONG型変数が4つある構造体
    RECT rc;
    //ウィンドウのクライアント領域の座標を取得する。描画するウィンドウの大きさを指定する
    GetClientRect(hwnd, &rc);

	//UINT = unsigned int
    //画面の横幅
    UINT width = rc.right - rc.left;
    //画面の縦幅
    UINT height = rc.bottom - rc.top;
    //デバイス生成フラグ
    UINT createDeviceFlags = 0;
    //MSAAを使うかのフラグ
    BOOL enable_4x_msaa = TRUE;
    //IDXGISwapChainの設定を行うための構造体。
    DXGI_SWAP_CHAIN_DESC sd;
    //sdの中身をクリアにする
    ZeroMemory(&sd, sizeof(sd));

    /*ここから1.*/
    //オフスクリーンはいくつか
    sd.BufferCount = 1;
    //横幅の解像度
    sd.BufferDesc.Width = width;
    //縦幅の解像度
    sd.BufferDesc.Height = height;
    //描画先の形式
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    /*ここから2.*/
    //描画先の更新頻度を分数形式で設定(Numeratorが分子、Denominatorが分母)
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    /*ここから3.*/
    //
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    //
    sd.SampleDesc.Count = 1;
    //MSAAの設定(MSAA = アンチエイリアス)
    sd.SampleDesc.Quality = 0;
    //ウィンドウモードであるか？(true:ウィンドウモード)
    sd.Windowed = TRUE;
    //ウィンドウハンドル
    sd.OutputWindow = hwnd;

/*TODO : 6-1 : デバイス・デバイスコンテキスト・スワップチェーンの作成------------------*/
    //ドライバーの一覧
    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,     //装置
        D3D_DRIVER_TYPE_WARP,         //IBMとMicroSoftが開発したOS
        D3D_DRIVER_TYPE_REFERENCE,    //ポインタみたいなもの、何かを参照しているもの
    };

    //driverTypesの配列の要素数を変数に代入する
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    //シェーダーモデルのレベル一覧
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,    //シェーダーモデル5を含むDirect3D 11.0でサポートされている機能を対象とする。
        D3D_FEATURE_LEVEL_10_1,    //シェーダーモデル4を含むDirect3D 10.1でサポートされている機能を対象とする。
        D3D_FEATURE_LEVEL_10_0,    //シェーダーモデル4を含むDirect3D 10.0でサポートされている機能を対象とする。
    };

    //シェーダーモデルのレベルを指定
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    //futureLevelsの配列の要素数を変数に代入する
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);


    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++) {
        //D3D11CreateDeviceAndSwapChain

        //Device     はDirectX11中で使用できるリソースを作成するのに用いる。各デバイス（GPU）でのリソース内容の違いを吸収する部分である
        //Context    は様々な状態を維持する。分かりやすく言えばコマンドバッファのようなものだ。GPUへの命令(コマンド）を溜め込み、描画命令でGPUへコマンドを送信し、稼働させる。
        //SwapChain  はGPUで描画した画像を実際の画面に反映させる機能、情報だ。GPUが描画する先は（大抵）今見ている画面そのものではなく、
        //           内部的に持っている仮想画面、裏画面、オフスクリーンに行われる。それをどのタイミングで、どのように実際の画面に反映させるかを担当している。

		//ドライバータイプを設定する
		driverType = driverTypes[driverTypeIndex];


        hr = D3D11CreateDeviceAndSwapChain(  //お待ちかねのCreateDeviceAndSwapChainのお時間だ。

            NULL,               //どのビデオアダプターを使用するか？
            driverType,         //ドライバのタイプを渡す
            NULL,               //上記をD3D_DRIVER_TYPE_SOFTWAREに設定した際に、その処理を行うDLLハンドルを渡す。それ以外を指定している際にはか鳴らすNULLを渡す
            createDeviceFlags,  //何らかのフラグを指定する
            featureLevels,      //D3D_FEATURE_LEVEL列挙型の配列を与える。
            numFeatureLevels,   //上記引数で、自分で定義した配列を与えていた場合、その配列の要素数をここに記述する。
            D3D11_SDK_VERSION,  //SDKのバージョン。必ずこの値。
            &sd,                //DXGI_SWAP_CHAIN_DESC構造体のアドレスを設定する。
            &swapChain,         //作成が成功した場合にそのSwapChainのアドレスを格納するポインタ変数へのアドレス。ここで指定したポインタ変数経由でSwapChainを操作する。
            &device,            //上記とほぼ同様で、こちらにはDeviceのポインタアドレスを設定する。
            &featureLevel,      //実際に作成に成功したD3D_FEATURE_LEVELを格納するためのD3D_FEAUTURE_LEVEL列挙型変数のアドレスを設定する。
            &deviceContext      //SwapChainやDeviceと同様で、こちらにはContextのポインタ変数のアドレスを設定する。

            );
        if (SUCCEEDED(hr))      //生成が成功したか判定。成功した場合for文から抜ける
            break;
    }
	if (FAILED(hr))             //アラート
		assert(hr);
	/*6-1 END----------------------------------------------------------------*/
    /*TODO : 6-2レンダーターゲットビューの作成----------------------------------*/

    //レンダーターゲットビューの作成に必要な変数・ω・？
    ID3D11Texture2D* pBackBuffer = NULL;
    //バックバッファ
    D3D11_TEXTURE2D_DESC back_buffer_desc;


    //SwapChainのBackBufferの一つにアクセスします
    hr = swapChain->GetBuffer(
        0,                          //このメゾットは最初のバッファのみアクセスする場合は、インデックスを0に設定する。
        __uuidof(ID3D11Texture2D),  //バッファーを操作するために使用されるインターフェースのタイプ
        (LPVOID*)&pBackBuffer       //バックバッファインターフェースへのポインタ
        );

	if (FAILED(hr))    //アラート
		assert(hr);

    //レンダーターゲットビューを作成する
    hr = device->CreateRenderTargetView(
        pBackBuffer,            //レンダリングターゲットを表すID3D11Resourceへのポインタ。
        NULL,                   //レンダリングターゲットビューの説明を表す。このパラメーターをNULLに設定すると、ミップマップレベル0のすべてのサブリソースにアクセスするビューが作成されます。
        &renderTaegetView       //ID3D11RenderTargetViewへのポインタのアドレス。
        );

    pBackBuffer->Release();
    if (FAILED(hr))
		assert(hr);

    pBackBuffer->GetDesc(&back_buffer_desc);

    /*6-2 END----------------------------------------------------------------*/
    /*TODO : 6-3深度ステンシルビューの作成-------------------------------------*/

    //深度ステンシルバッファ用のテクスチャを作成する。D3D11_TEXTURE2D_DESCで指定する
	D3D11_TEXTURE2D_DESC depth_stencil_buffer_desc = back_buffer_desc;

	ID3D11Texture2D* depth_stencil_buffer;
	depth_stencil_buffer_desc.Width = width;							//横幅
	depth_stencil_buffer_desc.Height = height;							//縦幅
    depth_stencil_buffer_desc.MipLevels = 1;                            //テクスチャー内のミップマップレベルの最大数。
    depth_stencil_buffer_desc.ArraySize = 1;                            //テクスチャー配列内のテクスチャーの数。
    depth_stencil_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;   //テキスチャーフォーマット。
    depth_stencil_buffer_desc.Usage = D3D11_USAGE_DEFAULT;              //テクスチャーの読み込みおよび書き込み方法を識別する値。最も一般的な値はD3D11_USAGE_DEFAULT。
    depth_stencil_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;     //パイプライン ステージへのバインドに関するフラグです
    depth_stencil_buffer_desc.CPUAccessFlags = 0;                       //許可するCPUアクセスの種類を指定するフラグ。アクセスが不要な場合は0を使用する。
    depth_stencil_buffer_desc.MiscFlags = 0;                            //他の一般性の低いリソースオプションを識別するフラグ。これらのフラグを適用しない場合は0を使用します。

    //深度ステンシルテクスチャの作成
	hr = device->CreateTexture2D(
		&depth_stencil_buffer_desc,				//D3D11_TEXTURE2D_DESC構造体へのポインタ。
		NULL,									//リソースがマルチサンプリングされている場合、マルチサンプリングされたリソースすは作成時にデータで初期化できないため,NULLでなければならない。
		&depth_stencil_buffer					//作成されたテクスチャID3D11Texture2Dインターフェイスへのポインタを受け取るバッファへのポインタ。
        );

    if (FAILED(hr))     //アラート
		assert(hr);

    //深度ステンシルビューを作成
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc = {};
    depth_stencil_view_desc.Format = depth_stencil_buffer_desc.Format;
    depth_stencil_view_desc.ViewDimension = enable_4x_msaa ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
    depth_stencil_view_desc.Flags = 0;
    depth_stencil_view_desc.Texture2D.MipSlice = 0;

    //深度ステンシルビューの設定
	hr = device->CreateDepthStencilView(depth_stencil_buffer, &depth_stencil_view_desc, &depthStencilView);
	if (FAILED(hr))     //アラート
		assert(hr);
	/*6-3 END ---------------------------------------------------------------*/


	/**********************************************************************
		テスト用の実体を生成する場所
	***********************************************************************/
	//オブジェクトを生成する

	SCENE_MANAGER->Init();

	srand((unsigned int)time(NULL));		//乱数系列の変更

	isMause = FALSE;

	return true;
}
void framework::update(float elapsed_time/*Elapsed seconds from last frame*/)
{
	/*if (GetAsyncKeyState('M') < 0)
	{
		if (!isMause) isMause = TRUE;
		else if (isMause) isMause = FALSE;
	}*/
	ShowCursor(isMause);
	if (GetAsyncKeyState(VK_F1) < 0)
	{
		PAD.InitInputDevice();
	}
	SCENE_MANAGER->Update();
}
void framework::render(float elapsed_time/*Elapsed seconds from last frame*/)
{
	/*TODO : 7-1 ビューポートの設定--------------------------------------------*/

	//ビューポートの構造体に値を設定する
	D3D11_VIEWPORT vp;
	vp.Width = SCREEN_WIDTH;
	vp.Height = SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	//デバイスコンテキストにビューポートを設定
	deviceContext->RSSetViewports(1, &vp);
	//レンダーターゲットのクリア

	//レンダーターゲットビューのクリアする色を決める
	FLOAT clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

	//レンダーターゲットビューをクリアする
	deviceContext->ClearRenderTargetView(renderTaegetView, clearColor);
	//深度ステンシルリソースのクリア
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//レンダーターゲットと深度ステンシルバッファのバインド
	deviceContext->OMSetRenderTargets(1, &renderTaegetView, depthStencilView);

	//BenchMark test
	{
		//		static float angle = 0;
		//		angle += 0.50f * elapsed_time; // 1 round per 60 seconds
		//
		//		static Blender blender(device);
		//
		//		static DirectX::XMFLOAT2 sprite_position[1024] = {};	// screen space
		//		static float timer = 0;									// 0 - 4(sec)
		//		timer += elapsed_time;
		//		if (timer > 4.0f)
		//		{
		//			for (auto& p : sprite_position)						// update positions once in four seconds
		//			{
		//				float a = static_cast<float>(rand()) / RAND_MAX*360.0f;	//angle(degree)
		//				float r = static_cast<float>(rand()) / RAND_MAX*256.0f;	//radius(screen space)
		//				p.x = cosf(a*0.01745f)*r;
		//				p.y = sinf(a*0.01745f)*r;
		//			}
		//			timer = 0;
		//		}
		//
		//		static benchmark bm;
		//		bm.begin();
		//
		//		deviceContext->OMSetBlendState(blender.modes[Blender::BLEND_MODE::BM_ADD], nullptr, 0xFFFFFFFF);
		//		for (auto& p : sprite_position)
		//		{
		//			particle->Render(deviceContext, p.x + 256, p.y + 256, 128, 128, 0, 0, 420, 420, angle * 4, 0.2f, 0.05f*timer, 0.01f*timer, fabsf(sinf(3.141592f*timer*0.5f*0.5f)));
		//		}
		//
		//
		//		float t = bm.end();
		//		static const int N = 10;
		//		static std::deque<float> queue(N, FLT_MAX);
		//		queue.push_back(t);
		//		queue.pop_front();
		//		decltype(queue)::iterator best = std::min_element(queue.begin(), queue.end());
		//		deviceContext->OMSetBlendState(blender.modes[Blender::BM_ADD], nullptr, 0xFFFFFFFF);
		//		font->textout(deviceContext, "benchmark t=" + std::to_string(*std::min_element(queue.begin(), queue.end())), 500, 0, 16, 16, 1, 1, 1, 1);
	}

	//Tentative rend to sprite

	static Blender blender(device);
	SetDrawBlendMode(BLEND_MODE::ALPHA);
//	deviceContext->OMSetBlendState(blender.modes[ALPHA], nullptr, 0xFFFFFFFF);

	SCENE_MANAGER->Render();


//#define USE3D
#ifdef USE3D

	//geometric_primitive:::Renderを呼び出す
		/**********************************************************************************************/
		//ここの中身は、そのうちモジュール化をしていくこと
		/**********************************************************************************************/
		/************************/
		//ワールド変換行列取得
		/************************/
	DirectX::XMMATRIX W, S, R, T, V;
	DirectX::XMFLOAT4X4	world_view_projection;
	DirectX::XMFLOAT4X4	world_m;
	DirectX::XMFLOAT4 color(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT4 light(1, -0.5f, 1, 0);
	{
		//	初期化
		W = DirectX::XMMatrixIdentity();
		//	拡大・縮小
		S = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
		//	回転
		R = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
		//	平行移動
		T = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		//	ワールド変換行列
		W = S * R * T;

		/***************************/
		//ビュー行列を設定
		/***************************/
		V = camera.GetViewMatrix();

		/****************************/
		//	プロジェクション行列を作成
		/****************************/
		float	fov = DirectX::XMConvertToRadians(30.0f);
		float	aspect = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
		DirectX::XMMATRIX	projection = DirectX::XMMatrixPerspectiveFovLH(fov, aspect, 0.1f, 1000.0f);
		//Matrix -> Float4x4 変換
		DirectX::XMStoreFloat4x4(&world_view_projection, W*V*projection);
		DirectX::XMStoreFloat4x4(&world_m, W);
	}
	/************************************************************************************************/
#endif
//	RenderMesh(world_view_projection, world_m, color, light);

	//レンダリングされたイメージの表示
	swapChain->Present(1, 0);
}



void	framework::PopInstance()
{
//	sprites[0] = new Sprite(device, L"logos.jpg");	//1920×1080
//	sprites[1] = new Sprite(device, L"moonoperater.png");	//900×877 //608*192
//	particle = new Sprite(device, L"particle-smoke.png");
//	font = new Sprite(device, L"./fonts/font0.png");
//	cube = new geometric_primitive(device);
//	batch = new SpriteBatch(device, L"logos.jpg",1);
//		mesh = new StaticMesh(device, L"Bison.obj");
//	mesh = new StaticMesh(device, L"Mr.Incredible.obj");
//	skinMesh = new SkinnedMesh(device, "002_cube.fbx");
//	box = new Box(device);
}

void	framework::RenderSprite()
{
//		static Blender blender(device);
//		deviceContext->OMSetBlendState(blender.modes[ADD], nullptr, 0xFFFFFFFF);

	//	deviceContext->OMSetBlendState(blender.modes[Blender::BM_ALPHA], nullptr, 0xFFFFFFFF);
	//	deviceContext->OMSetBlendState(blender.modes[Blender::BM_NONE], nullptr, 0xFFFFFFFF);
	//	sprites[1]->Render(deviceContext, 8, 8, 608, 192, 0, 0, 608, 192);
	//	deviceContext->OMSetBlendState(blender.modes[Blender::BM_NONE], nullptr, 0xFFFFFFFF);
	//	batch->Begin(deviceContext);
	//	for (int i = 0; i < 100; i++)
	//		batch->Draw(0+ i*5 , 0, 1280 + i * 5, 720, 0, 0, 1920, 1080, 1920 / 2, 1080 / 2, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f);
	//	batch->Draw(DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(608, 192), DirectX::XMFLOAT2(0, 0), DirectX::XMFLOAT2(608, 192), DirectX::XMFLOAT2(608 / 2, 192 / 2), 0.0f, DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));
	//	batch->End(deviceContext);
	//box->Render(deviceContext, 100, 100, 50, 50);
}

void	framework::RenderMesh(DirectX::XMFLOAT4X4 _WVP, DirectX::XMFLOAT4X4 _W, DirectX::XMFLOAT4 _materialColor, DirectX::XMFLOAT4 _lightDirection)
{
//	cube->Render(deviceContext, _WVP, _W, _materialColor, _lightDirection, false);
//	mesh->Render(deviceContext, _WVP, _W, _lightDirection, _materialColor, false);
	skinMesh->Render(deviceContext, _WVP, _W, _lightDirection, _materialColor, false);
}


