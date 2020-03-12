#include"GeometricPrimitive.h"
#include"shader.h"
geometric_primitive::geometric_primitive(ID3D11Device* _device)
{
	HRESULT hr = S_OK;

	//���_�V�F�[�_�[�I�u�W�F�N�g�̐���
	D3D11_INPUT_ELEMENT_DESC input_element_desc[] =
	{
		/*�����o�[�Љ�*/
		//semanticName			: �V�F�[�_�[��input-signature�ł��̗v�f�Ɋ֘A�t�����Ă���HLSL�Z�}���e�B�N�X
		//semanticIndex			: �����Z�}���e�B�b�N�����v�f����������ꍇ�ɂ̂ݕK�v
		//Format				: DXGI_FORMAT���Q��
		//InputSlot				: ���̓A���Z�u�������ʂ��鐮���l�ł��B�L��������0�`15
		//AlignedByteoffset		: �I�v�V�����ł��B�֋X��AD3D111_APPEND_ALIGNED_ELEMENT���g�p����
		//InputSlotClass		: �P����̓X���b�g�̓��̓f�[�^�N���X�����ʂ��܂��B
		//InstanceDataStepRate	: �o�b�t�@�����P�v�f�i�ޑO�ɓ����C���X�^���X���Ƃ̃f�[�^���g�p���ĕ`�悷��C���X�^���X�̐�
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
//		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	create_vs_from_cso(_device, "geometric_primitive_vs.cso", &vertexShader, &inputLayout, input_element_desc, ARRAYSIZE(input_element_desc));

	//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g�̍쐬
	create_ps_from_cso(_device, "geometric_primitive_ps.cso", &pixelShader);

	//create rasterizer state : solid mode
	{
		D3D11_RASTERIZER_DESC rasterizer_desc = {};
		rasterizer_desc.FillMode = D3D11_FILL_SOLID; //D3D11_FILL_WIREFRAME, D3D11_FILL_SOLID
		rasterizer_desc.CullMode = D3D11_CULL_BACK; //D3D11_CULL_NONE, D3D11_CULL_FRONT, D3D11_CULL_BACK   
		rasterizer_desc.FrontCounterClockwise = FALSE;
		rasterizer_desc.DepthBias = 0;
		rasterizer_desc.DepthBiasClamp = 0;
		rasterizer_desc.SlopeScaledDepthBias = 0;
		rasterizer_desc.DepthClipEnable = TRUE;
		rasterizer_desc.ScissorEnable = FALSE;
		rasterizer_desc.MultisampleEnable = FALSE;
		rasterizer_desc.AntialiasedLineEnable = FALSE;
		hr = _device->CreateRasterizerState(&rasterizer_desc, &renderWire);
		assert(SUCCEEDED(hr) && "Wire�̃��X�^���C�U�X�e�[�g����Ă���~�܂�����");
	}
	// create rasterizer state : wireframe mode
	{
		D3D11_RASTERIZER_DESC rasterizer_desc = {};
		rasterizer_desc.FillMode = D3D11_FILL_WIREFRAME; //D3D11_FILL_WIREFRAME, D3D11_FILL_SOLID
		rasterizer_desc.CullMode = D3D11_CULL_BACK; //D3D11_CULL_NONE, D3D11_CULL_FRONT, D3D11_CULL_BACK   
		rasterizer_desc.FrontCounterClockwise = FALSE;
		rasterizer_desc.DepthBias = 0;
		rasterizer_desc.DepthBiasClamp = 0;
		rasterizer_desc.SlopeScaledDepthBias = 0;
		rasterizer_desc.DepthClipEnable = TRUE;
		rasterizer_desc.ScissorEnable = FALSE;
		rasterizer_desc.MultisampleEnable = FALSE;
		rasterizer_desc.AntialiasedLineEnable = FALSE;
		hr = _device->CreateRasterizerState(&rasterizer_desc, &renderSolid);
		assert(SUCCEEDED(hr) && "Solid�̃��X�^���C�U�X�e�[�g����Ă���~�܂�����");
	}

	//�[�x�X�e���V���X�e�[�g�Ɏg���\���̂̍쐬
	D3D11_DEPTH_STENCIL_DESC depth_stencil_desc = {};
	depth_stencil_desc.DepthEnable = TRUE;
	depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depth_stencil_desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	depth_stencil_desc.StencilEnable = FALSE;

	//�����I
	hr = _device->CreateDepthStencilState(&depth_stencil_desc, &depthStencilState);
	assert(SUCCEEDED(hr) && "�[�x�X�e���V���X�e�[�g����Ă���~�܂�����");

	//�萔�o�b�t�@�I�u�W�F�N�g�̐���
	{
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.ByteWidth = sizeof(cbuffer);
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;

		hr = _device->CreateBuffer(&buffer_desc, nullptr, &constantBuffer);
		assert(SUCCEEDED(hr) && "�萔�o�b�t�@����Ă���~�܂�����");
	}

	vertex vertices[4 * 6] = {};			//��ʂ�����4���_ �~ �����̂͘Z�ʁ@�W���_ �~ �R
	unsigned int indices[3 * 2 * 6] = {};	//�`�悷��O�p�`�̐� �~ 3

	int numV = 0, numI = 0;
	//�����ō�����ق�
	{
		/*-----------------< ��� >-----------------*/
		//���_���
		vertices[numV + 0].position = DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f);
		vertices[numV + 1].position = DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f);
		vertices[numV + 2].position = DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f);
		vertices[numV + 3].position = DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f);
		//indexBuffer���
		indices[numI + 0] = numV + 0;
		indices[numI + 1] = numV + 1;
		indices[numI + 2] = numV + 3;
		indices[numI + 3] = numV + 1;
		indices[numI + 4] = numV + 2;
		indices[numI + 5] = numV + 3;
		//�@���͏����
		vertices[numV + 0].normal = vertices[numV + 1].normal = vertices[numV + 2].normal = vertices[numV + 3].normal = DirectX::XMFLOAT3(0.0f, +1.0f, 0.0f);	

		numV += 4; numI += 6;


		/*-----------------< ���� >-----------------*/
		//���_���
		vertices[numV + 0].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
		vertices[numV + 1].position = DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f);
		vertices[numV + 2].position = DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f);
		vertices[numV + 3].position = DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f);
		//indexBuffer���
		indices[numI + 0] = numV + 0;
		indices[numI + 1] = numV + 1;
		indices[numI + 2] = numV + 3;
		indices[numI + 3] = numV + 1;
		indices[numI + 4] = numV + 2;
		indices[numI + 5] = numV + 3;
		//�@���͉�����
		vertices[numV + 0].normal = vertices[numV + 1].normal = vertices[numV + 2].normal = vertices[numV + 3].normal = DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f);

		numV += 4; numI += 6;

		//TODO : ��������
		/*-----------------< ���� >-----------------*/
		//���_���
		vertices[numV + 0].position = DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f);
		vertices[numV + 1].position = DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f);
		vertices[numV + 2].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
		vertices[numV + 3].position = DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f);
		//indexBuffer���
		indices[numI + 0] = numV + 0;
		indices[numI + 1] = numV + 1;
		indices[numI + 2] = numV + 3;
		indices[numI + 3] = numV + 1;
		indices[numI + 4] = numV + 2;
		indices[numI + 5] = numV + 3;
		//�@���͍�����
		vertices[numV + 0].normal = vertices[numV + 1].normal = vertices[numV + 2].normal = vertices[numV + 3].normal = DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f);

		numV += 4; numI += 6;


		/*-----------------< �E�� >-----------------*/
		//���_���
		vertices[numV + 0].position = DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f);
		vertices[numV + 1].position = DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f);
		vertices[numV + 2].position = DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f);
		vertices[numV + 3].position = DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f);
		//indexBuffer���
		indices[numI + 0] = numV + 0;
		indices[numI + 1] = numV + 1;
		indices[numI + 2] = numV + 3;
		indices[numI + 3] = numV + 1;
		indices[numI + 4] = numV + 2;
		indices[numI + 5] = numV + 3;
		//�@���͉E����
		vertices[numV + 0].normal = vertices[numV + 1].normal = vertices[numV + 2].normal = vertices[numV + 3].normal = DirectX::XMFLOAT3(+1.0f, 0.0f, 0.0f);

		numV += 4; numI += 6;


		/*-----------------< �O�� >-----------------*/
		//���_���
		vertices[numV + 0].position = DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f);
		vertices[numV + 1].position = DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f);
		vertices[numV + 2].position = DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f);
		vertices[numV + 3].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
		//indexBuffer���
		indices[numI + 0] = numV + 0;
		indices[numI + 1] = numV + 1;
		indices[numI + 2] = numV + 3;
		indices[numI + 3] = numV + 1;
		indices[numI + 4] = numV + 2;
		indices[numI + 5] = numV + 3;
		//�@���͑O����
		vertices[numV + 0].normal = vertices[numV + 1].normal = vertices[numV + 2].normal = vertices[numV + 3].normal = DirectX::XMFLOAT3(0.0f, 0.0f, +1.0f);

		numV += 4; numI += 6;


		/*-----------------< ��� >-----------------*/
		//���_���
		vertices[numV + 0].position = DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f);
		vertices[numV + 1].position = DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f);
		vertices[numV + 2].position = DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f);
		vertices[numV + 3].position = DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f);
		//indexBuffer���
		indices[numI + 0] = numV + 0;
		indices[numI + 1] = numV + 1;
		indices[numI + 2] = numV + 3;
		indices[numI + 3] = numV + 1;
		indices[numI + 4] = numV + 2;
		indices[numI + 5] = numV + 3;
		//�@���͌����
		vertices[numV + 0].normal = vertices[numV + 1].normal = vertices[numV + 2].normal = vertices[numV + 3].normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);

		numV += 4; numI += 6;
	}
	//�搶�̂���{
	{
		//	���
//		vertices[numV + 0].position = DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f);
//		vertices[numV + 1].position = DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f);
//		vertices[numV + 2].position = DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f);
//		vertices[numV + 3].position = DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f);
//		vertices[numV + 0].normal = vertices[numV + 1].normal =
//			vertices[numV + 2].normal =
//			vertices[numV + 3].normal = DirectX::XMFLOAT3(+0.0f, +1.0f, +0.0f);
//		indices[numI + 0] = numV + 0;	indices[numI + 1] = numV + 1;	indices[numI + 2] = numV + 2;
//		indices[numI + 3] = numV + 1;	indices[numI + 4] = numV + 3;	indices[numI + 5] = numV + 2;
//		numV += 4;	numI += 6;
//
//		//	����
//		vertices[numV + 0].position = DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f);
//		vertices[numV + 1].position = DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f);
//		vertices[numV + 2].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
//		vertices[numV + 3].position = DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f);
//		vertices[numV + 0].normal = vertices[numV + 1].normal =
//			vertices[numV + 2].normal =
//			vertices[numV + 3].normal = DirectX::XMFLOAT3(+0.0f, -1.0f, +0.0f);
//		indices[numI + 0] = numV + 0;	indices[numI + 1] = numV + 2;	indices[numI + 2] = numV + 1;
//		indices[numI + 3] = numV + 1;	indices[numI + 4] = numV + 2;	indices[numI + 5] = numV + 3;
//		numV += 4;	numI += 6;
//
//		//	�E��
//		vertices[numV + 0].position = DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f);
//		vertices[numV + 1].position = DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f);
//		vertices[numV + 2].position = DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f);
//		vertices[numV + 3].position = DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f);
//		vertices[numV + 0].normal = vertices[numV + 1].normal =
//			vertices[numV + 2].normal =
//			vertices[numV + 3].normal = DirectX::XMFLOAT3(+1.0f, +0.0f, +0.0f);
//		indices[numI + 0] = numV + 0;	indices[numI + 1] = numV + 1;	indices[numI + 2] = numV + 2;
//		indices[numI + 3] = numV + 1;	indices[numI + 4] = numV + 3;	indices[numI + 5] = numV + 2;
//		numV += 4;	numI += 6;
//
//		//	����
//		vertices[numV + 0].position = DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f);
//		vertices[numV + 1].position = DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f);
//		vertices[numV + 2].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
//		vertices[numV + 3].position = DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f);
//		vertices[numV + 0].normal = vertices[numV + 1].normal =
//			vertices[numV + 2].normal =
//			vertices[numV + 3].normal = DirectX::XMFLOAT3(-1.0f, +0.0f, +0.0f);
//		indices[numI + 0] = numV + 0;	indices[numI + 1] = numV + 2;	indices[numI + 2] = numV + 1;
//		indices[numI + 3] = numV + 1;	indices[numI + 4] = numV + 2;	indices[numI + 5] = numV + 3;
//		numV += 4;	numI += 6;
//
//		//	���
//		vertices[numV + 0].position = DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f);
//		vertices[numV + 1].position = DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f);
//		vertices[numV + 2].position = DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f);
//		vertices[numV + 3].position = DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f);
//		vertices[numV + 0].normal = vertices[numV + 1].normal =
//			vertices[numV + 2].normal =
//			vertices[numV + 3].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, +1.0f);
//		indices[numI + 0] = numV + 0;	indices[numI + 1] = numV + 1;	indices[numI + 2] = numV + 2;
//		indices[numI + 3] = numV + 1;	indices[numI + 4] = numV + 3;	indices[numI + 5] = numV + 2;
//		numV += 4;	numI += 6;
//
//		//	�O��
//		vertices[numV + 0].position = DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f);
//		vertices[numV + 1].position = DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f);
//		vertices[numV + 2].position = DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f);
//		vertices[numV + 3].position = DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f);
//		vertices[numV + 0].normal = vertices[numV + 1].normal =
//			vertices[numV + 2].normal =
//			vertices[numV + 3].normal = DirectX::XMFLOAT3(+0.0f, +0.0f, -1.0f);
//		indices[numI + 0] = numV + 0;	indices[numI + 1] = numV + 2;	indices[numI + 2] = numV + 1;
//		indices[numI + 3] = numV + 1;	indices[numI + 4] = numV + 2;	indices[numI + 5] = numV + 3;
//		numV += 4;	numI += 6;
	}
	/***************************************************************************************************/
	CreateBuffers(_device, vertices, numV, indices, numI);
}

void	geometric_primitive::CreateBuffers(ID3D11Device* _device, vertex* _vertices, int _numVertices, u_int* _indices, int _numIndices)
{
	HRESULT hr;

	//�o�[�e�b�N�X�o�b�t�@�̐���
	{
		D3D11_BUFFER_DESC buffer_desc = {};
		D3D11_SUBRESOURCE_DATA subresource_data = {};

		buffer_desc.ByteWidth = sizeof(vertex)*_numVertices;
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		//buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		subresource_data.pSysMem = _vertices;
		subresource_data.SysMemPitch = 0; //Not use for vertex buffers.mm 
		subresource_data.SysMemSlicePitch = 0; //Not use for vertex buffers.

		hr = _device->CreateBuffer(&buffer_desc, &subresource_data, &vertexBuffer);
		assert(SUCCEEDED(hr) && "vertex�o�b�t�@����Ă���~�܂�����");
	}

	//�C���f�b�N�X�o�b�t�@�̐���
	{
		D3D11_BUFFER_DESC buffer_desc = {};
		D3D11_SUBRESOURCE_DATA subresource_data = {};

		buffer_desc.ByteWidth = sizeof(u_int)*_numIndices;
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		//buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		subresource_data.pSysMem = _indices;
		subresource_data.SysMemPitch = 0; //Not use for index buffers.
		subresource_data.SysMemSlicePitch = 0; //Not use for index buffers.
		hr = _device->CreateBuffer(&buffer_desc, &subresource_data, &indexBuffer);
		assert(SUCCEEDED(hr) && "index�o�b�t�@����Ă���~�܂�����");
	}
}

void	geometric_primitive::Render(ID3D11DeviceContext* _deviceContext, const DirectX::XMFLOAT4X4 & _wvp, const DirectX::XMFLOAT4X4 & _world, const DirectX::XMFLOAT4 & _color, const DirectX::XMFLOAT4 & _light,bool bSolid)
{
	cbuffer cb;
	cb.world_view_projection = _wvp;
	cb.world = _world;
	cb.light_direction = _light;
	cb.material_color = _color;
	//�萔�o�b�t�@�̃o�C���h
	_deviceContext->UpdateSubresource(constantBuffer, 0, 0, &cb, 0, 0);
	_deviceContext->VSSetConstantBuffers(0, 1, &constantBuffer);

	UINT stride = sizeof(vertex);
	UINT offset = 0;

	//���_�o�b�t�@�̃o�C���h
	_deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	//�C���f�b�N�X�o�b�t�@�̃o�C���h
	_deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//�v���~�e�B�u�g�|���W�[�̐ݒ�
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//���̓��C�A�E�g�I�u�W�F�N�g�̃o�C���h
	_deviceContext->IASetInputLayout(
		inputLayout		//ID3D11InputLayout�̃|�C���^
		);

	//�V�F�[�_�̐ݒ�
	//VertexShader
	_deviceContext->VSSetShader(
		vertexShader,	//���_�V�F�[�_�ւ̃|�C���^
		nullptr,		//�N���X�C���X�^���X�C���^�[�t�F�[�X�̔z��ւ̃|�C���^�B�V�F�[�_���C���^�[�t�F�[�X���g�p���Ȃ��ꍇ��NULL
		0				//�z����̃N���X�C���X�^���X�C���^�[�t�F�[�X�̐�
		);
	//PixelShader
	_deviceContext->PSSetShader(
		pixelShader,	//�s�N�Z���V�F�[�_�ւ̃|�C���^
		nullptr,		//�N���X�C���X�^���X�C���^�[�t�F�[�X�̔z��ւ̃|�C���^�B�V�F�[�_���C���^�[�t�F�[�X���g�p���Ȃ��ꍇ��NULL
		0				//�z����̃N���X�C���X�^���X�C���^�[�t�F�[�X�̐�
		);

	//�[�x�X�e���V���X�e�[�g�̐ݒ�
	_deviceContext->OMSetDepthStencilState(
		depthStencilState,
		1
		);

	//���X�^���C�U�[�X�e�[�g�̐ݒ�
	if (bSolid)
	{
		_deviceContext->RSSetState(
			renderSolid		//�p�C�v���C���Ƀo�C���h���邽�߂̃��X�^���C�U��ԃC���^�t�F�[�X�ւ̃|�C���^
			);
	}
	else
	{
		_deviceContext->RSSetState(
			renderWire		//�p�C�v���C���Ƀo�C���h���邽�߂̃��X�^���C�U��ԃC���^�t�F�[�X�ւ̃|�C���^
			);
	}

	//�C���f�b�N�X�t������Ă���v���~�e�B�u�̕`��
//	_deviceContext->DrawIndexed(6, 0, 0);
	D3D11_BUFFER_DESC buffer_desc;
	indexBuffer->GetDesc(&buffer_desc);
	_deviceContext->DrawIndexed(buffer_desc.ByteWidth / sizeof(int), 0, 0);


}
