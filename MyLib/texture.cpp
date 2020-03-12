#include"texture.h"
#include"misc.h"
#include<WICTextureLoader.h>
#include<wrl.h>
#include<map>
#include<assert.h>

HRESULT load_texture_from_file(ID3D11Device* device, const wchar_t* file_name, ID3D11ShaderResourceView**	shaderResourceView, D3D11_TEXTURE2D_DESC* texture2d_desc)
{
	HRESULT hr = S_OK;
	Microsoft::WRL::ComPtr<ID3D11Resource> resource;

	static std::map < std::wstring, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> cache;
	auto it = cache.find(file_name);
	if (it != cache.end())
	{
		*shaderResourceView = it->second.Get();
		(*shaderResourceView)->AddRef();
		(*shaderResourceView)->GetResource(resource.GetAddressOf());
	}
	else
	{
		hr = DirectX::CreateWICTextureFromFile(device, file_name, resource.GetAddressOf(), shaderResourceView);
		if (FAILED(hr))
			assert(hr);

		cache.insert(std::make_pair(file_name, *shaderResourceView));
	}

	// 指定されたIID(Interface ID)に応じて、そのインターフェースポインタを
	// *ppvObjectに渡してやります。
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2d;
	hr = resource.Get()->QueryInterface<ID3D11Texture2D>(texture2d.GetAddressOf());
	if (FAILED(hr))
		assert(hr);
	texture2d->GetDesc(texture2d_desc);

	return hr;
}