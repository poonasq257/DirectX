#ifndef _MODEL_H_
#define _MODEL_H_

#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
using namespace std;

#include "Texture.h"

class Model
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
private:
	ID3D11Buffer* vertexBuffer, *indexBuffer;
	int vertexCount, indexCount;
	Texture* texture;
	ModelType* model;
private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

	bool LoadModel(char*);
	void ReleaseModel();
public:
	Model();
	Model(const Model& other);
	~Model();
public:
	bool Initialize(ID3D11Device*, char*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

};

#endif