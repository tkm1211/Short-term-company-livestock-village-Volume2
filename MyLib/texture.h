#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include<d3d11.h>

HRESULT load_texture_from_file(ID3D11Device* device, const wchar_t* file_name, ID3D11ShaderResourceView** sharderResourceView, D3D11_TEXTURE2D_DESC* texture2d_desc);


#endif //!_TEXTURE_H_
