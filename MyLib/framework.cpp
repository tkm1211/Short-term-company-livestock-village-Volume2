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
/*TODO6 : framework�N���X��initialize�����o�֐��ňȉ��̏�������������*/
/*---------------------------------------------------------------*/
    //1. �`���̏c���T�C�Y�A�t�H�[�}�b�g(�`��)��ݒ�
    //
    //2. �`���̍X�V�p�x(60fps��������30fps��������Ƃ�������)�𕪐��`���Őݒ�
    //
    //3. ���A�`���̎g�p���@(�܂������ł�DXGI_USAGE_RENDER_TARGET_OUTPUT���낤)�A�`���E�B���h�E�̃n���h���A�E�B���h�E���[�h���ۂ��AMSAA�̃N�I���e�B��J�E���g����ݒ�
    //
    //4. �����܂ł�SwapChain�̃p�����[�^�ݒ�B����Device�AContext�̔\�̓��x����ݒ肷��BD3D_FEATURE_LEVEL_11_0��DirectX 11�̊�����S�ɖ�������GPU��œ��삷�郌�x���ƂȂ�B��������Ȃ��ꍇ��10_1 -> 10_0 -> 9_3 -> 9_2 -> 9_1�ƁA���X�ɉ����Ă��������s���A�������邱�Ƃ��F��B

//static�ϐ��̐錾
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
    //HRESULT = 0(true)=�����A0x00000000�`oxffffffff(false)=���s
    HRESULT hr = S_OK;
    //RECT = LONG�^�ϐ���4����\����
    RECT rc;
    //�E�B���h�E�̃N���C�A���g�̈�̍��W���擾����B�`�悷��E�B���h�E�̑傫�����w�肷��
    GetClientRect(hwnd, &rc);

	//UINT = unsigned int
    //��ʂ̉���
    UINT width = rc.right - rc.left;
    //��ʂ̏c��
    UINT height = rc.bottom - rc.top;
    //�f�o�C�X�����t���O
    UINT createDeviceFlags = 0;
    //MSAA���g�����̃t���O
    BOOL enable_4x_msaa = TRUE;
    //IDXGISwapChain�̐ݒ���s�����߂̍\���́B
    DXGI_SWAP_CHAIN_DESC sd;
    //sd�̒��g���N���A�ɂ���
    ZeroMemory(&sd, sizeof(sd));

    /*��������1.*/
    //�I�t�X�N���[���͂�����
    sd.BufferCount = 1;
    //�����̉𑜓x
    sd.BufferDesc.Width = width;
    //�c���̉𑜓x
    sd.BufferDesc.Height = height;
    //�`���̌`��
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    /*��������2.*/
    //�`���̍X�V�p�x�𕪐��`���Őݒ�(Numerator�����q�ADenominator������)
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    /*��������3.*/
    //
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    //
    sd.SampleDesc.Count = 1;
    //MSAA�̐ݒ�(MSAA = �A���`�G�C���A�X)
    sd.SampleDesc.Quality = 0;
    //�E�B���h�E���[�h�ł��邩�H(true:�E�B���h�E���[�h)
    sd.Windowed = TRUE;
    //�E�B���h�E�n���h��
    sd.OutputWindow = hwnd;

/*TODO : 6-1 : �f�o�C�X�E�f�o�C�X�R���e�L�X�g�E�X���b�v�`�F�[���̍쐬------------------*/
    //�h���C�o�[�̈ꗗ
    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,     //���u
        D3D_DRIVER_TYPE_WARP,         //IBM��MicroSoft���J������OS
        D3D_DRIVER_TYPE_REFERENCE,    //�|�C���^�݂����Ȃ��́A�������Q�Ƃ��Ă������
    };

    //driverTypes�̔z��̗v�f����ϐ��ɑ������
    UINT numDriverTypes = ARRAYSIZE(driverTypes);

    //�V�F�[�_�[���f���̃��x���ꗗ
    D3D_FEATURE_LEVEL featureLevels[] =
    {
        D3D_FEATURE_LEVEL_11_0,    //�V�F�[�_�[���f��5���܂�Direct3D 11.0�ŃT�|�[�g����Ă���@�\��ΏۂƂ���B
        D3D_FEATURE_LEVEL_10_1,    //�V�F�[�_�[���f��4���܂�Direct3D 10.1�ŃT�|�[�g����Ă���@�\��ΏۂƂ���B
        D3D_FEATURE_LEVEL_10_0,    //�V�F�[�_�[���f��4���܂�Direct3D 10.0�ŃT�|�[�g����Ă���@�\��ΏۂƂ���B
    };

    //�V�F�[�_�[���f���̃��x�����w��
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    //futureLevels�̔z��̗v�f����ϐ��ɑ������
    UINT numFeatureLevels = ARRAYSIZE(featureLevels);


    for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++) {
        //D3D11CreateDeviceAndSwapChain

        //Device     ��DirectX11���Ŏg�p�ł��郊�\�[�X���쐬����̂ɗp����B�e�f�o�C�X�iGPU�j�ł̃��\�[�X���e�̈Ⴂ���z�����镔���ł���
        //Context    �͗l�X�ȏ�Ԃ��ێ�����B������₷�������΃R�}���h�o�b�t�@�̂悤�Ȃ��̂��BGPU�ւ̖���(�R�}���h�j�𗭂ߍ��݁A�`�施�߂�GPU�փR�}���h�𑗐M���A�ғ�������B
        //SwapChain  ��GPU�ŕ`�悵���摜�����ۂ̉�ʂɔ��f������@�\�A��񂾁BGPU���`�悷���́i���j�����Ă����ʂ��̂��̂ł͂Ȃ��A
        //           �����I�Ɏ����Ă��鉼�z��ʁA����ʁA�I�t�X�N���[���ɍs����B������ǂ̃^�C�~���O�ŁA�ǂ̂悤�Ɏ��ۂ̉�ʂɔ��f�����邩��S�����Ă���B

		//�h���C�o�[�^�C�v��ݒ肷��
		driverType = driverTypes[driverTypeIndex];


        hr = D3D11CreateDeviceAndSwapChain(  //���҂����˂�CreateDeviceAndSwapChain�̂����Ԃ��B

            NULL,               //�ǂ̃r�f�I�A�_�v�^�[���g�p���邩�H
            driverType,         //�h���C�o�̃^�C�v��n��
            NULL,               //��L��D3D_DRIVER_TYPE_SOFTWARE�ɐݒ肵���ۂɁA���̏������s��DLL�n���h����n���B����ȊO���w�肵�Ă���ۂɂ͂��炷NULL��n��
            createDeviceFlags,  //���炩�̃t���O���w�肷��
            featureLevels,      //D3D_FEATURE_LEVEL�񋓌^�̔z���^����B
            numFeatureLevels,   //��L�����ŁA�����Œ�`�����z���^���Ă����ꍇ�A���̔z��̗v�f���������ɋL�q����B
            D3D11_SDK_VERSION,  //SDK�̃o�[�W�����B�K�����̒l�B
            &sd,                //DXGI_SWAP_CHAIN_DESC�\���̂̃A�h���X��ݒ肷��B
            &swapChain,         //�쐬�����������ꍇ�ɂ���SwapChain�̃A�h���X���i�[����|�C���^�ϐ��ւ̃A�h���X�B�����Ŏw�肵���|�C���^�ϐ��o�R��SwapChain�𑀍삷��B
            &device,            //��L�Ƃقړ��l�ŁA������ɂ�Device�̃|�C���^�A�h���X��ݒ肷��B
            &featureLevel,      //���ۂɍ쐬�ɐ�������D3D_FEATURE_LEVEL���i�[���邽�߂�D3D_FEAUTURE_LEVEL�񋓌^�ϐ��̃A�h���X��ݒ肷��B
            &deviceContext      //SwapChain��Device�Ɠ��l�ŁA������ɂ�Context�̃|�C���^�ϐ��̃A�h���X��ݒ肷��B

            );
        if (SUCCEEDED(hr))      //��������������������B���������ꍇfor�����甲����
            break;
    }
	if (FAILED(hr))             //�A���[�g
		assert(hr);
	/*6-1 END----------------------------------------------------------------*/
    /*TODO : 6-2�����_�[�^�[�Q�b�g�r���[�̍쐬----------------------------------*/

    //�����_�[�^�[�Q�b�g�r���[�̍쐬�ɕK�v�ȕϐ��E�ցE�H
    ID3D11Texture2D* pBackBuffer = NULL;
    //�o�b�N�o�b�t�@
    D3D11_TEXTURE2D_DESC back_buffer_desc;


    //SwapChain��BackBuffer�̈�ɃA�N�Z�X���܂�
    hr = swapChain->GetBuffer(
        0,                          //���̃��]�b�g�͍ŏ��̃o�b�t�@�̂݃A�N�Z�X����ꍇ�́A�C���f�b�N�X��0�ɐݒ肷��B
        __uuidof(ID3D11Texture2D),  //�o�b�t�@�[�𑀍삷�邽�߂Ɏg�p�����C���^�[�t�F�[�X�̃^�C�v
        (LPVOID*)&pBackBuffer       //�o�b�N�o�b�t�@�C���^�[�t�F�[�X�ւ̃|�C���^
        );

	if (FAILED(hr))    //�A���[�g
		assert(hr);

    //�����_�[�^�[�Q�b�g�r���[���쐬����
    hr = device->CreateRenderTargetView(
        pBackBuffer,            //�����_�����O�^�[�Q�b�g��\��ID3D11Resource�ւ̃|�C���^�B
        NULL,                   //�����_�����O�^�[�Q�b�g�r���[�̐�����\���B���̃p�����[�^�[��NULL�ɐݒ肷��ƁA�~�b�v�}�b�v���x��0�̂��ׂẴT�u���\�[�X�ɃA�N�Z�X����r���[���쐬����܂��B
        &renderTaegetView       //ID3D11RenderTargetView�ւ̃|�C���^�̃A�h���X�B
        );

    pBackBuffer->Release();
    if (FAILED(hr))
		assert(hr);

    pBackBuffer->GetDesc(&back_buffer_desc);

    /*6-2 END----------------------------------------------------------------*/
    /*TODO : 6-3�[�x�X�e���V���r���[�̍쐬-------------------------------------*/

    //�[�x�X�e���V���o�b�t�@�p�̃e�N�X�`�����쐬����BD3D11_TEXTURE2D_DESC�Ŏw�肷��
	D3D11_TEXTURE2D_DESC depth_stencil_buffer_desc = back_buffer_desc;

	ID3D11Texture2D* depth_stencil_buffer;
	depth_stencil_buffer_desc.Width = width;							//����
	depth_stencil_buffer_desc.Height = height;							//�c��
    depth_stencil_buffer_desc.MipLevels = 1;                            //�e�N�X�`���[���̃~�b�v�}�b�v���x���̍ő吔�B
    depth_stencil_buffer_desc.ArraySize = 1;                            //�e�N�X�`���[�z����̃e�N�X�`���[�̐��B
    depth_stencil_buffer_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;   //�e�L�X�`���[�t�H�[�}�b�g�B
    depth_stencil_buffer_desc.Usage = D3D11_USAGE_DEFAULT;              //�e�N�X�`���[�̓ǂݍ��݂���я������ݕ��@�����ʂ���l�B�ł���ʓI�Ȓl��D3D11_USAGE_DEFAULT�B
    depth_stencil_buffer_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;     //�p�C�v���C�� �X�e�[�W�ւ̃o�C���h�Ɋւ���t���O�ł�
    depth_stencil_buffer_desc.CPUAccessFlags = 0;                       //������CPU�A�N�Z�X�̎�ނ��w�肷��t���O�B�A�N�Z�X���s�v�ȏꍇ��0���g�p����B
    depth_stencil_buffer_desc.MiscFlags = 0;                            //���̈�ʐ��̒Ⴂ���\�[�X�I�v�V���������ʂ���t���O�B�����̃t���O��K�p���Ȃ��ꍇ��0���g�p���܂��B

    //�[�x�X�e���V���e�N�X�`���̍쐬
	hr = device->CreateTexture2D(
		&depth_stencil_buffer_desc,				//D3D11_TEXTURE2D_DESC�\���̂ւ̃|�C���^�B
		NULL,									//���\�[�X���}���`�T���v�����O����Ă���ꍇ�A�}���`�T���v�����O���ꂽ���\�[�X���͍쐬���Ƀf�[�^�ŏ������ł��Ȃ�����,NULL�łȂ���΂Ȃ�Ȃ��B
		&depth_stencil_buffer					//�쐬���ꂽ�e�N�X�`��ID3D11Texture2D�C���^�[�t�F�C�X�ւ̃|�C���^���󂯎��o�b�t�@�ւ̃|�C���^�B
        );

    if (FAILED(hr))     //�A���[�g
		assert(hr);

    //�[�x�X�e���V���r���[���쐬
	D3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc = {};
    depth_stencil_view_desc.Format = depth_stencil_buffer_desc.Format;
    depth_stencil_view_desc.ViewDimension = enable_4x_msaa ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
    depth_stencil_view_desc.Flags = 0;
    depth_stencil_view_desc.Texture2D.MipSlice = 0;

    //�[�x�X�e���V���r���[�̐ݒ�
	hr = device->CreateDepthStencilView(depth_stencil_buffer, &depth_stencil_view_desc, &depthStencilView);
	if (FAILED(hr))     //�A���[�g
		assert(hr);
	/*6-3 END ---------------------------------------------------------------*/


	/**********************************************************************
		�e�X�g�p�̎��̂𐶐�����ꏊ
	***********************************************************************/
	//�I�u�W�F�N�g�𐶐�����

	SCENE_MANAGER->Init();

	srand((unsigned int)time(NULL));		//�����n��̕ύX

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
	/*TODO : 7-1 �r���[�|�[�g�̐ݒ�--------------------------------------------*/

	//�r���[�|�[�g�̍\���̂ɒl��ݒ肷��
	D3D11_VIEWPORT vp;
	vp.Width = SCREEN_WIDTH;
	vp.Height = SCREEN_HEIGHT;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;

	//�f�o�C�X�R���e�L�X�g�Ƀr���[�|�[�g��ݒ�
	deviceContext->RSSetViewports(1, &vp);
	//�����_�[�^�[�Q�b�g�̃N���A

	//�����_�[�^�[�Q�b�g�r���[�̃N���A����F�����߂�
	FLOAT clearColor[4] = { 0.0f,0.0f,0.0f,0.0f };

	//�����_�[�^�[�Q�b�g�r���[���N���A����
	deviceContext->ClearRenderTargetView(renderTaegetView, clearColor);
	//�[�x�X�e���V�����\�[�X�̃N���A
	deviceContext->ClearDepthStencilView(depthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	//�����_�[�^�[�Q�b�g�Ɛ[�x�X�e���V���o�b�t�@�̃o�C���h
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

	//geometric_primitive:::Render���Ăяo��
		/**********************************************************************************************/
		//�����̒��g�́A���̂������W���[���������Ă�������
		/**********************************************************************************************/
		/************************/
		//���[���h�ϊ��s��擾
		/************************/
	DirectX::XMMATRIX W, S, R, T, V;
	DirectX::XMFLOAT4X4	world_view_projection;
	DirectX::XMFLOAT4X4	world_m;
	DirectX::XMFLOAT4 color(1.0f, 1.0f, 1.0f, 1.0f);
	DirectX::XMFLOAT4 light(1, -0.5f, 1, 0);
	{
		//	������
		W = DirectX::XMMatrixIdentity();
		//	�g��E�k��
		S = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
		//	��]
		R = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f);
		//	���s�ړ�
		T = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
		//	���[���h�ϊ��s��
		W = S * R * T;

		/***************************/
		//�r���[�s���ݒ�
		/***************************/
		V = camera.GetViewMatrix();

		/****************************/
		//	�v���W�F�N�V�����s����쐬
		/****************************/
		float	fov = DirectX::XMConvertToRadians(30.0f);
		float	aspect = static_cast<float>(SCREEN_WIDTH) / static_cast<float>(SCREEN_HEIGHT);
		DirectX::XMMATRIX	projection = DirectX::XMMatrixPerspectiveFovLH(fov, aspect, 0.1f, 1000.0f);
		//Matrix -> Float4x4 �ϊ�
		DirectX::XMStoreFloat4x4(&world_view_projection, W*V*projection);
		DirectX::XMStoreFloat4x4(&world_m, W);
	}
	/************************************************************************************************/
#endif
//	RenderMesh(world_view_projection, world_m, color, light);

	//�����_�����O���ꂽ�C���[�W�̕\��
	swapChain->Present(1, 0);
}



void	framework::PopInstance()
{
//	sprites[0] = new Sprite(device, L"logos.jpg");	//1920�~1080
//	sprites[1] = new Sprite(device, L"moonoperater.png");	//900�~877 //608*192
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


