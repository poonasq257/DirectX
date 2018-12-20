#include "Model.h"

Model::Model() : vertexBuffer(nullptr), indexBuffer(nullptr), texture(nullptr), model(nullptr) {}

Model::Model(const Model& other) {}

Model::~Model() {}

bool Model::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	// Load in the model data,
	if (!LoadModel(modelFilename)) return false;

	// Initialize the vertex and index buffers.
	if (!InitializeBuffers(device)) return false;

	// Load the texture for this model.
	if (!LoadTexture(device, textureFilename)) return false;

	return true;
}

void Model::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();
}

void Model::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);
}


int Model::GetIndexCount()
{
	return indexCount;
}


ID3D11ShaderResourceView* Model::GetTexture()
{
	return texture->GetTexture();
}


bool Model::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;

	// Create the vertex array.
	vertices = new VertexType[vertexCount];
	if (!vertices) return false;

	// Create the index array.
	indices = new unsigned long[indexCount];
	if (!indices) return false;

	// Load the vertex array and index array with data.
	for(int i = 0; i < vertexCount; i++)
	{
		vertices[i].position = D3DXVECTOR3(model[i].x, model[i].y, model[i].z);
		vertices[i].texture = D3DXVECTOR2(model[i].tu, model[i].tv);

		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	if (FAILED(device->CreateBuffer(&vertexBufferDesc,
		&vertexData, &vertexBuffer))) return false;

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	if (FAILED(device->CreateBuffer(&indexBufferDesc,
		&indexData, &indexBuffer))) return false;

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


void Model::ShutdownBuffers()
{
	// Release the index buffer.
	if(indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = 0;
	}
}


void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}


bool Model::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	// Create the texture object.
	texture = new Texture;
	if (!texture) return false;

	// Initialize the texture object.
	return texture->Initialize(device, filename);
}


void Model::ReleaseTexture()
{
	// Release the texture object.
	if(texture)
	{
		texture->Shutdown();
		delete texture;
		texture = 0;
	}
}


bool Model::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	
	// Open the model file.  If it could not open the file then exit.
	fin.open(filename);
	if (fin.fail()) return false;

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':') fin.get(input);

	// Read in the vertex count.
	fin >> vertexCount;

	// Set the number of indices to be the same as the vertex count.
	indexCount = vertexCount;

	// Create the model using the vertex count that was read in.
	model = new ModelType[vertexCount];
	if (!model) return false;

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':') fin.get(input);

	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for(int i = 0; i < vertexCount; i++)
	{
		fin >> model[i].x >> model[i].y >> model[i].z;
		fin >> model[i].tu >> model[i].tv;
		fin >> model[i].nx >> model[i].ny >> model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}


void Model::ReleaseModel()
{
	if(model)
	{
		delete [] model;
		model = 0;
	}
}