#ifndef _GEOMETRIC_PRIMITIVE_H_
#define _GEOMETRIC_PRIMITIVE_H_

#include<DirectXMath.h>
#include<d3d11.h>
/******************************************
	Geometric Primitive
******************************************/
class geometric_primitive
{
public:
	/******************************
	頂点フォーマット
	*******************************/
	struct vertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
	};

	/******************************
	定数バッファ
	******************************/
	struct cbuffer
	{
		DirectX::XMFLOAT4X4 world_view_projection;	//ワールド・ビュー・プロジェクション合成行列
		DirectX::XMFLOAT4X4 world;
		DirectX::XMFLOAT4 material_color;
		DirectX::XMFLOAT4 light_direction;
	};
public:
	//頂点シェーダー
	ID3D11VertexShader*			vertexShader;
	//ピクセルシェーダー
	ID3D11PixelShader*			pixelShader;
	//入力レイアウト
	ID3D11InputLayout*			inputLayout;
	//頂点バッファ
	ID3D11Buffer*				vertexBuffer;
	//インデックスバッファ
	ID3D11Buffer*				indexBuffer;
	//定数バッファ
	ID3D11Buffer*				constantBuffer;
	//線描画
	ID3D11RasterizerState*		renderWire;
	//塗りつぶし
	ID3D11RasterizerState*		renderSolid;
	//深度ステンシルステート
	ID3D11DepthStencilState*	depthStencilState;

	geometric_primitive(ID3D11Device*);
	~geometric_primitive()
	{
		if (vertexShader)
			vertexShader->Release();
		if (pixelShader)
			pixelShader->Release();
		if (inputLayout)
			inputLayout->Release();
		if (vertexBuffer)
			vertexBuffer->Release();
		if (indexBuffer)
			indexBuffer->Release();
		if (constantBuffer)
			constantBuffer->Release();
		if (renderWire)
			renderWire->Release();
		if (renderSolid)
			renderSolid->Release();
		if (depthStencilState)
			depthStencilState->Release();
		vertexShader = nullptr;
		pixelShader = nullptr;
		inputLayout = nullptr;
		vertexBuffer = nullptr;
		indexBuffer = nullptr;
		constantBuffer = nullptr;
		renderWire = nullptr;
		renderSolid = nullptr;
		depthStencilState = nullptr;
	}

public:
	void						Render(
		ID3D11DeviceContext*		_deviceContext,	//deviceContext
		const DirectX::XMFLOAT4X4 &	_wvp,			//world view projection合成行列
		const DirectX::XMFLOAT4X4 &	_world,			//world 変換座標
		const DirectX::XMFLOAT4 &	_color,			//材質color
		const DirectX::XMFLOAT4 &	_light,			//ライトの方向
		bool						bSolid			//線・塗りつぶしrender flag
		);

protected:
	void						CreateBuffers(
		ID3D11Device*	_device,		//ID3D11Deviceのポインタ
		vertex*			_vertices,		//vertex配列の先頭アドレス
		int				_numVertices,	//頂点の数
		u_int*			_indices,		//indicesの先頭アドレス
		int				_numIndices		//indexの数
		);

};


#endif //!_GEOMETRIC_PRIMITIVE_H_