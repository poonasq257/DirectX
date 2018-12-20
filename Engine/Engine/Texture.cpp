#include "Texture.h"

Texture::Texture() : texture(nullptr) {}

Texture::Texture(const Texture& other) {}

Texture::~Texture() {}

bool Texture::Initialize(ID3D11Device* device, const WCHAR* fileName)
{
	// Load the texture in.
	if (FAILED(D3DX11CreateShaderResourceViewFromFile(device, fileName,
		NULL, NULL, &texture, NULL))) return false;
	return true;
}


void Texture::Shutdown()
{
	// Release the texture resource.
	if(texture)
	{
		texture->Release();
		texture = 0;
	}
}

ID3D11ShaderResourceView* Texture::GetTexture()
{
	return texture;
}