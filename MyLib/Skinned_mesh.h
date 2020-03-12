#pragma once

#include <d3d11.h>
#include <wrl.h>
#include <directxmath.h>
#include <vector>

class SkinnedMesh
{
	struct vertex
	{
		DirectX::XMFLOAT3	position;
		DirectX::XMFLOAT3	normal;
		DirectX::XMFLOAT2	tex;
		DirectX::XMFLOAT2	uv;
	};
	struct cbuffer
	{
		DirectX::XMFLOAT4X4	worldViewProjection;
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4	materialColor;
		DirectX::XMFLOAT4	lightDirection;
	};
	struct subset
	{
		std::wstring usemtl;
		u_int index_start = 0;	// start number of index buffer
		u_int index_count = 0;	// number of bertices (indices)
	};
	std::vector<subset> subsets;

	struct material
	{
		DirectX::XMFLOAT4 color = { 0.8f, 0.8f, 0.8f,1.0f };
		ID3D11ShaderResourceView* shaderResourceView;
	};
	material diffuse;

private:
	ID3D11Buffer*				vertexBuffer;
	ID3D11Buffer*				indexBuffer;
	ID3D11Buffer*				constantBuffer;
	ID3D11VertexShader*			vertexShader;
	ID3D11PixelShader*			pixelShader;
	ID3D11InputLayout*			inputLayout;
	ID3D11RasterizerState*		rasterizerState[2];
	ID3D11DepthStencilState*	depthStencilState;

	ID3D11ShaderResourceView*	shaderResourceView;
	D3D11_TEXTURE2D_DESC		texture2d;
	ID3D11SamplerState*			samplerState;


public:
	SkinnedMesh(ID3D11Device* _device, const char* _fbx_filename);
	virtual ~SkinnedMesh();

	void Render(ID3D11DeviceContext* _context, const DirectX::XMFLOAT4X4 &_WVP, const DirectX::XMFLOAT4X4 &_world, const DirectX::XMFLOAT4 &_lightDirection, DirectX::XMFLOAT4 &_materialColor, bool wireframe = false);
	void LoadTextureFile(const char*, ID3D11ShaderResourceView**);
private:
	void CreateBuffer(ID3D11Device* _device, vertex*_verices, int _numVertices, u_int* _indices, int _numIndices);
//	void LoadMtlFile(wchar_t* _mtlFileName);
//	void LoadTextureFile(wchar_t*, ID3D11ShaderResourceView**);


};