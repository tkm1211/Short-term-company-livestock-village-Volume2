#define _CRT_SECURE_NO_WARNINGS
#include "Skinned_mesh.h"
#include "misc.h"
#include <fbxsdk.h>
#include <vector>
#include <functional>
#include "shader.h"
#include "texture.h"
#include "framework.h"

using namespace fbxsdk;

SkinnedMesh::SkinnedMesh(ID3D11Device* _device, const char* _fbx_filename)
{
	//Create the FBX SDK manager
	FbxManager* manager = FbxManager::Create();

	//Create an IOSettings object. IOSROOT is defined in Fbxiosettingspath.h
	manager->SetIOSettings(FbxIOSettings::Create(manager, IOSROOT));

	//Create an inporter
	FbxImporter* importer = FbxImporter::Create(manager, "");

	//Initialize the importer
	bool import_status = false;
	import_status = importer->Initialize(_fbx_filename, -1, manager->GetIOSettings());
	_ASSERT_EXPR_A(import_status, importer->GetStatus().GetErrorString());

	//Create a new scene so it can be populated by the imported file.
	FbxScene* scene = FbxScene::Create(manager, "");

	//Import the contents of the file into the scene.
	import_status = importer->Import(scene);

	_ASSERT_EXPR_A(import_status, importer->GetStatus().GetErrorString());

	//Convert mesh, NURBS and patch into triangle mesh
	fbxsdk::FbxGeometryConverter geometry_converter(manager);
	geometry_converter.Triangulate(scene, /*repllace*/true);

	//Fetch node attributes and materials under this node recursively. Currently only mesh.
	std::vector<FbxNode*>fetched_meshes;
	std::function<void(FbxNode*)>traverse = [&](FbxNode* node)
	{
		if (node)
		{
			FbxNodeAttribute* fbx_node_attribute = node->GetNodeAttribute();
			if (fbx_node_attribute)
			{
				switch (fbx_node_attribute->GetAttributeType())
				{
				case FbxNodeAttribute::eMesh:
					fetched_meshes.push_back(node);
					break;
				}
			}
			for (int i = 0; i < node->GetChildCount(); i++)
				traverse(node->GetChild(i));
		}
	};
	traverse(scene->GetRootNode());

	fbxsdk::FbxMesh* fbx_mesh = fetched_meshes.at(0)->GetMesh();	//Currently only one mesh

	//Fetch mesh data
	std::vector<vertex>vertices;	//Vertex buffer
	std::vector<u_int>indices;		//Index buffer
	u_int vertex_count = 0;
	const FbxVector4* array_of_control_points = fbx_mesh->GetControlPoints();
	const int number_of_polygons = fbx_mesh->GetPolygonCount();

	FbxStringList uv_names;
	fbx_mesh->GetUVSetNames(uv_names);

	for (int index_of_polygon = 0; index_of_polygon < number_of_polygons; index_of_polygon++)
	{
		for (int index_of_vertex = 0; index_of_vertex < 3; index_of_vertex++)
		{

			vertex vertex;
			const int index_of_control_point = fbx_mesh->GetPolygonVertex(index_of_polygon, index_of_vertex);
			vertex.position.x = static_cast<float>(array_of_control_points[index_of_control_point][0]);
			vertex.position.y = static_cast<float>(array_of_control_points[index_of_control_point][1]);
			vertex.position.z = static_cast<float>(array_of_control_points[index_of_control_point][2]);

			FbxVector4 normal;
			fbx_mesh->GetPolygonVertexNormal(index_of_polygon, index_of_vertex, normal);
			vertex.normal.x = static_cast<float>(normal[0]);
			vertex.normal.y = static_cast<float>(normal[1]);
			vertex.normal.z = static_cast<float>(normal[2]);

			FbxVector2 uv;
			bool unmapped_uv;
			fbx_mesh->GetPolygonVertexUV(index_of_polygon, index_of_vertex, uv_names[0], uv, unmapped_uv);
			vertex.tex.x = static_cast<float>(uv[0]);
			vertex.tex.y = 1.0f - static_cast<float>(uv[1]);

			vertices.push_back(vertex);
			indices.push_back(vertex_count);
			vertex_count += 1;
		}
	}

	//Fetch material properties
	const int number_of_materials = fbx_mesh->GetNode()->GetMaterialCount();
	for (int index_of_material = 0; index_of_material < number_of_materials; ++index_of_material)
	{
		const FbxSurfaceMaterial* surface_material = fbx_mesh->GetNode()->GetMaterial(index_of_material);

		const FbxProperty property = surface_material->FindProperty(FbxSurfaceMaterial::sDiffuse);
		const FbxProperty factor = surface_material->FindProperty(FbxSurfaceMaterial::sDiffuseFactor);
		if (property.IsValid() && factor.IsValid())
		{
			FbxDouble3 color = property.Get<FbxDouble3>();
			double f = factor.Get<FbxDouble>();

			diffuse.color.x = static_cast<float>(color[0] * f);
			diffuse.color.y = static_cast<float>(color[1] * f);
			diffuse.color.z = static_cast<float>(color[2] * f);
			diffuse.color.w = 1.0f;
		}
		if (property.IsValid())
		{
			const int number_of_textures = property.GetSrcObjectCount<FbxFileTexture>();
			if (number_of_textures)
			{
				const FbxFileTexture* file_texture = property.GetSrcObject<FbxFileTexture>();
				if (file_texture)
				{
					const char* filename = file_texture->GetRelativeFileName();

					//Create "diffuse.shader_resource_view" from "filename".
					LoadTextureFile(filename, &diffuse.shaderResourceView);
				}
			}
		}
	}

	manager->Destroy();

	//Initialize Direct3D COM objects

	CreateBuffer(_device, vertices.data(), vertices.size(), indices.data(), indices.size());

	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC input_element_desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },

	};

	create_vs_from_cso(_device, "skinned_mesh_vs.cso", &vertexShader, &inputLayout, input_element_desc, ARRAYSIZE(input_element_desc));
	create_ps_from_cso(_device, "skinned_mesh_ps.cso", &pixelShader);

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
		hr = _device->CreateRasterizerState(&rasterizer_desc, &rasterizerState[0]);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
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
		hr = _device->CreateRasterizerState(&rasterizer_desc, &rasterizerState[1]);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}
	// create depth stencil state
	{
		D3D11_DEPTH_STENCIL_DESC depth_stencil_desc;
		depth_stencil_desc.DepthEnable = TRUE;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
		depth_stencil_desc.StencilEnable = FALSE;
		depth_stencil_desc.StencilReadMask = 0xFF;
		depth_stencil_desc.StencilWriteMask = 0xFF;
		depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
		depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
		depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		hr = _device->CreateDepthStencilState(&depth_stencil_desc, &depthStencilState);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}
	// create constant buffer
	{
		D3D11_BUFFER_DESC buffer_desc = {};
		buffer_desc.ByteWidth = sizeof(cbuffer);
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		hr = _device->CreateBuffer(&buffer_desc, nullptr, &constantBuffer);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}
}

SkinnedMesh::~SkinnedMesh()
{
	vertexBuffer->Release();
	indexBuffer->Release();
	constantBuffer->Release();
	vertexShader->Release();
	pixelShader->Release();
	inputLayout->Release();
	rasterizerState[0]->Release();
	rasterizerState[1]->Release();
	depthStencilState->Release();
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
	constantBuffer = nullptr;
	vertexShader = nullptr;
	pixelShader = nullptr;
	inputLayout = nullptr;
	rasterizerState[0] = nullptr;
	rasterizerState[1] = nullptr;
	depthStencilState = nullptr;
}

void	SkinnedMesh::CreateBuffer(ID3D11Device *device, vertex *vertices, int num_vertices, u_int *indices, int num_indices)
{
	HRESULT hr;
	{
		D3D11_BUFFER_DESC buffer_desc = {};
		D3D11_SUBRESOURCE_DATA subresource_data = {};

		buffer_desc.ByteWidth = sizeof(vertex)*num_vertices;
		buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		//buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
		buffer_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		subresource_data.pSysMem = vertices;
		subresource_data.SysMemPitch = 0; //Not use for vertex buffers.mm 
		subresource_data.SysMemSlicePitch = 0; //Not use for vertex buffers.

		hr = device->CreateBuffer(&buffer_desc, &subresource_data, &vertexBuffer);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}
	{
		D3D11_BUFFER_DESC buffer_desc = {};
		D3D11_SUBRESOURCE_DATA subresource_data = {};

		buffer_desc.ByteWidth = sizeof(u_int)*num_indices;
		//buffer_desc.Usage = D3D11_USAGE_DEFAULT;
		buffer_desc.Usage = D3D11_USAGE_IMMUTABLE;
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		buffer_desc.CPUAccessFlags = 0;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		subresource_data.pSysMem = indices;
		subresource_data.SysMemPitch = 0; //Not use for index buffers.
		subresource_data.SysMemSlicePitch = 0; //Not use for index buffers.
		hr = device->CreateBuffer(&buffer_desc, &subresource_data, &indexBuffer);
		_ASSERT_EXPR(SUCCEEDED(hr), hr_trace(hr));
	}
}

void	SkinnedMesh::Render(ID3D11DeviceContext* _context, const DirectX::XMFLOAT4X4 &_WVP, const DirectX::XMFLOAT4X4 &_world, const DirectX::XMFLOAT4 &_lightDirection, DirectX::XMFLOAT4 &_materialColor, bool wireframe)
{
#if 0
	for (auto& material : materials)
	{
		cbuffer data;
		data.worldViewProjection = _WVP;
		data.world = _world;
		data.lightDirection = _lightDirection;
		data.materialColor.x = _materialColor.x*material.Kd.x;
		data.materialColor.y = _materialColor.y*material.Kd.y;
		data.materialColor.z = _materialColor.z*material.Kd.z;
		_context->UpdateSubresource(constantBuffer, 0, 0, &data, 0, 0);
		_context->VSSetConstantBuffers(0, 1, &constantBuffer);
		_context->PSSetShaderResources(0, 1, &material.shaderResourceView);
		_context->PSSetSamplers(0, 1, &samplerState);

		u_int stride = sizeof(vertex);
		u_int offset = 0;
		_context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
		_context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
		_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		_context->IASetInputLayout(inputLayout);

		_context->VSSetShader(vertexShader, nullptr, 0);
		_context->PSSetShader(pixelShader, nullptr, 0);

		_context->OMSetDepthStencilState(depthStencilState, 1);
		if (wireframe)
		{
			_context->RSSetState(rasterizerState[1]);
		}
		else
		{
			_context->RSSetState(rasterizerState[0]);
		}

		for (auto& subset : subsets)
		{
			if (material.newmtl == subset.usemtl)
			{
				_context->DrawIndexed(subset.index_count, subset.index_start, 0);
			}

		}
	}
#else
	cbuffer data;
	data.worldViewProjection = _WVP;
	data.world = _world;
	data.lightDirection = _lightDirection;
	data.materialColor = _materialColor;
	_context->UpdateSubresource(constantBuffer, 0, 0, &data, 0, 0);
	_context->VSSetConstantBuffers(0, 1, &constantBuffer);

	u_int stride = sizeof(vertex);
	u_int offset = 0;
	_context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	_context->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_context->IASetInputLayout(inputLayout);

	_context->PSSetShaderResources(0, 1, &diffuse.shaderResourceView);
	_context->PSSetSamplers(0, 1, &samplerState);

	_context->VSSetShader(vertexShader, nullptr, 0);
	_context->PSSetShader(pixelShader, nullptr, 0);

	_context->OMSetDepthStencilState(depthStencilState, 1);
	if (wireframe)
	{
		_context->RSSetState(rasterizerState[1]);
	}
	else
	{
		_context->RSSetState(rasterizerState[0]);
	}

	D3D11_BUFFER_DESC buffer_desc;
	indexBuffer->GetDesc(&buffer_desc);
	_context->DrawIndexed(buffer_desc.ByteWidth / sizeof(u_int), 0, 0);
#endif
}

void SkinnedMesh::LoadTextureFile(const char* _fileName, ID3D11ShaderResourceView** _shaderResourceView)
{
	HRESULT hr = S_OK;
	wchar_t name[100];

	setlocale(LC_CTYPE, "jpn");
	mbstowcs(name, _fileName, 100);
	load_texture_from_file(FRAMEWORK->GetDevice(), name, _shaderResourceView, &texture2d);
}