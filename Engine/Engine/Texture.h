#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

#include <d3d11.h>
#include <d3dx11tex.h>

class Texture
{
private:
	ID3D11ShaderResourceView* texture;
public:
	Texture();
	Texture(const Texture& other);
	~Texture();
public:
	bool Initialize(ID3D11Device* device, const WCHAR* fileName);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
};

#endif