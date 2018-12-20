//////////////////////////////////////////////////////////////////////////////////////////////////
// File: silhouetteEdges.cpp
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "silhouetteEdges.h"

const DWORD MeshVertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;

SilhouetteEdges::SilhouetteEdges(IDirect3DDevice9* device,
	ID3DXMesh* mesh,
	ID3DXBuffer* adjBuffer)
{
	_device = device;
	_vb = 0;
	_ib = 0;
	_decl = 0;
	_numVerts = 0;
	_numFaces = 0;
	genEdgeVertices(mesh, adjBuffer);
	genEdgeIndices(mesh);
	createVertexDeclaration();
}



SilhouetteEdges::~SilhouetteEdges()
{
	d3d::Release<IDirect3DVertexBuffer9*>(_vb);
	d3d::Release<IDirect3DIndexBuffer9*>(_ib);
	d3d::Release<IDirect3DVertexDeclaration9*>(_decl);
}


bool SilhouetteEdges::createVertexDeclaration()
{
	HRESULT hr = 0;
	// 버텍스 속성 구조체 정의.
	// 하나의 위치 벡터와 3개의 법선 벡터로 구성된 버텍스 포맷을 기술한다.
	D3DVERTEXELEMENT9 decl[] =
	{
		{ 0,  0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		// 위치 벡터의 포맷.
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
		// 법선 벡터uc2의포맷.
		{ 0, 24, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   1 },
		// 법선 벡터uc2의포맷.
		{ 0, 36, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   2 },
		// 법선 벡터uc2의포맷.
		D3DDECL_END()
	};

	hr = _device->CreateVertexDeclaration(decl, &_decl);

	// 위에서 선언한 IDirect3DVertexDeclaration9 인터페이스로의 포인터를 얻는다.
	if (FAILED(hr))
	{
		::MessageBox(0, L"CreateVertexDeclaration() - FAILED", 0, 0);
		return false;
	}
	return true;
}

// 면의 법선 벡터를 계산.
void SilhouetteEdges::getFaceNormal(ID3DXMesh* mesh,
	DWORD faceIndex,
	D3DXVECTOR3* faceNormal)
{
	MeshVertex* vertices = 0;
	mesh->LockVertexBuffer(0, (void**)&vertices);
	WORD* indices = 0;
	mesh->LockIndexBuffer(0, (void**)&indices);

	// 삼각형의 인덱스를 계산.
	WORD index0 = indices[faceIndex * 3];
	WORD index1 = indices[faceIndex * 3 + 1];
	WORD index2 = indices[faceIndex * 3 + 2];

	// 현재 삼각형의 정점위치를 추출.
	D3DXVECTOR3 v0 = vertices[index0].position;
	D3DXVECTOR3 v1 = vertices[index1].position;
	D3DXVECTOR3 v2 = vertices[index2].position;

	// 면의 법선 벡터를 계산.
	// 용책 412p.
	D3DXVECTOR3 edge0, edge1;
	edge0 = v1 - v0;
	edge1 = v2 - v0;
	D3DXVec3Cross(faceNormal, &edge0, &edge1);
	D3DXVec3Normalize(faceNormal, faceNormal);

	mesh->UnlockVertexBuffer();
	mesh->UnlockIndexBuffer();
}



// 면 경계 테스트(외곽선인지 테스트).
// 용책 413p 참고.
void SilhouetteEdges::getFaceNormals(ID3DXMesh*   mesh,
	ID3DXBuffer* adjBuffer,
	D3DXVECTOR3* currentFaceNormal,
	D3DXVECTOR3  adjFaceNormals[3],
	DWORD        faceIndex)
{
	MeshVertex* vertices = 0;
	mesh->LockVertexBuffer(0, (void**)&vertices);
	WORD* indices = 0;
	mesh->LockIndexBuffer(0, (void**)&indices);
	DWORD* adj = (DWORD*)adjBuffer->GetBufferPointer();

	// 면의 법선 벡터를 계산.
	getFaceNormal(mesh, faceIndex, currentFaceNormal);

	// 인접 면의 인덱스를 구함.
	WORD faceIndex0 = adj[faceIndex * 3];
	WORD faceIndex1 = adj[faceIndex * 3 + 1];
	WORD faceIndex2 = adj[faceIndex * 3 + 2];

	//
	// Get adjacent face normals, if there is no adjacent face,
	// then set the adjacent face normal to the opposite of the
	// "currentFaceNormal".  Recall we do this because edges that
	// don't have an adjacent triangle are automatically considered
	// silhouette edges.  And in order to make that happen, we need
	// the current face normal and adjacent face normal to point
	// in the opposite direction.  Also, recall that an entry
	// in the adjacency buffer equal to -1 denotes that the edge
	// doesn't have an adjacent triangle.

	D3DXVECTOR3 faceNormal0, faceNormal1, faceNormal2;

	if (faceIndex0 != USHRT_MAX) // is there an adjacent triangle?
	{
		WORD i0 = indices[faceIndex0 * 3];
		WORD i1 = indices[faceIndex0 * 3 + 1];
		WORD i2 = indices[faceIndex0 * 3 + 2];
		D3DXVECTOR3 v0 = vertices[i0].position;
		D3DXVECTOR3 v1 = vertices[i1].position;
		D3DXVECTOR3 v2 = vertices[i2].position;
		D3DXVECTOR3 edge0 = v1 - v0;
		D3DXVECTOR3 edge1 = v2 - v0;
		D3DXVec3Cross(&faceNormal0, &edge0, &edge1);
		D3DXVec3Normalize(&faceNormal0, &faceNormal0);
	}
	else
	{
		faceNormal0 = -(*currentFaceNormal);
	}
	if (faceIndex1 != USHRT_MAX) // is there an adjacent triangle?
	{
		WORD i0 = indices[faceIndex1 * 3];
		WORD i1 = indices[faceIndex1 * 3 + 1];
		WORD i2 = indices[faceIndex1 * 3 + 2];

		D3DXVECTOR3 v0 = vertices[i0].position;
		D3DXVECTOR3 v1 = vertices[i1].position;
		D3DXVECTOR3 v2 = vertices[i2].position;

		D3DXVECTOR3 edge0 = v1 - v0;
		D3DXVECTOR3 edge1 = v2 - v0;
		D3DXVec3Cross(&faceNormal1, &edge0, &edge1);
		D3DXVec3Normalize(&faceNormal1, &faceNormal1);
	}
	else
	{
		faceNormal1 = -(*currentFaceNormal);
	}

	if (faceIndex2 != USHRT_MAX) // is there an adjacent triangle?
	{
		WORD i0 = indices[faceIndex2 * 3];
		WORD i1 = indices[faceIndex2 * 3 + 1];
		WORD i2 = indices[faceIndex2 * 3 + 2];

		D3DXVECTOR3 v0 = vertices[i0].position;
		D3DXVECTOR3 v1 = vertices[i1].position;
		D3DXVECTOR3 v2 = vertices[i2].position;

		D3DXVECTOR3 edge0 = v1 - v0;
		D3DXVECTOR3 edge1 = v2 - v0;
		D3DXVec3Cross(&faceNormal2, &edge0, &edge1);
		D3DXVec3Normalize(&faceNormal2, &faceNormal2);
	}
	else
	{
		faceNormal2 = -(*currentFaceNormal);
	}

	// save adjacent face normals
	adjFaceNormals[0] = faceNormal0;
	adjFaceNormals[1] = faceNormal1;
	adjFaceNormals[2] = faceNormal2;

	mesh->UnlockVertexBuffer();
	mesh->UnlockIndexBuffer();
}



// 버텍스가 외곽선에 있는지 검사.
void SilhouetteEdges::genEdgeVertices(ID3DXMesh* mesh,
	ID3DXBuffer* adjBuffer)
{
	// 3 edges per face and 4 vertices per edge
	_numVerts = mesh->GetNumFaces() * 3 * 4;
	_device->CreateVertexBuffer(
		_numVerts * sizeof(EdgeVertex),
		D3DUSAGE_WRITEONLY,
		0, // using vertex declaration
		D3DPOOL_MANAGED,
		&_vb,
		0);

	MeshVertex* vertices = 0;
	mesh->LockVertexBuffer(0, (void**)&vertices);

	WORD* indices = 0;
	mesh->LockIndexBuffer(0, (void**)&indices);

	EdgeVertex* edgeVertices = 0;
	_vb->Lock(0, 0, (void**)&edgeVertices, 0);

	// 메쉬 표면 수 만큼 루프돌면서,
	//
	for (int i = 0; i < mesh->GetNumFaces(); i++)
	{
		D3DXVECTOR3 currentFaceNormal;
		D3DXVECTOR3 adjFaceNormals[3];

		// 면 경계 테스트(외곽선인지 테스트).
		getFaceNormals(mesh, adjBuffer, &currentFaceNormal, adjFaceNormals, i);

		// 면의 인덱스를 얻는다.
		WORD index0 = indices[i * 3];
		WORD index1 = indices[i * 3 + 1];
		WORD index2 = indices[i * 3 + 2];

		// 면의 정점을 얻는다.
		MeshVertex v0 = vertices[index0];
		MeshVertex v1 = vertices[index1];
		MeshVertex v2 = vertices[index2];

		// A        B
		// *--------*
		// |  edge  |
		// *--------*
		// C        D
		// note, C and D are duplicates of A and B respectively,
		// such that the quad is degenerate.  The vertex shader
		// will un-degenerate the quad if it is a silhouette edge.
		// compute edge0 v0->v1, note adjacent face
		// normal is faceNormal0
		EdgeVertex A0, B0, C0, D0;

		A0.position = v0.position;
		A0.normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		A0.faceNormal1 = currentFaceNormal;
		A0.faceNormal2 = adjFaceNormals[0];

		B0.position = v1.position;
		B0.normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		B0.faceNormal1 = currentFaceNormal;
		B0.faceNormal2 = adjFaceNormals[0];


		C0 = A0;
		C0.normal = v0.normal;

		D0 = B0;
		D0.normal = v1.normal;

		*edgeVertices = A0; ++edgeVertices;
		*edgeVertices = B0; ++edgeVertices;
		*edgeVertices = C0; ++edgeVertices;
		*edgeVertices = D0; ++edgeVertices;


		// compute edge0 v1->v2, note adjacent face
		// normal is faceNormal1
		EdgeVertex A1, B1, C1, D1;


		A1.position = v1.position;
		A1.normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		A1.faceNormal1 = currentFaceNormal;
		A1.faceNormal2 = adjFaceNormals[1];

		B1.position = v2.position;
		B1.normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		B1.faceNormal1 = currentFaceNormal;
		B1.faceNormal2 = adjFaceNormals[1];

		C1 = A1;
		C1.normal = v1.normal;

		D1 = B1;
		D1.normal = v2.normal;

		*edgeVertices = A1; ++edgeVertices;
		*edgeVertices = B1; ++edgeVertices;
		*edgeVertices = C1; ++edgeVertices;
		*edgeVertices = D1; ++edgeVertices;

		// compute edge0 v0->v2, note adjacent face
		// normal is faceNormal2
		EdgeVertex A2, B2, C2, D2;

		A2.position = v0.position;
		A2.normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		A2.faceNormal1 = currentFaceNormal;
		A2.faceNormal2 = adjFaceNormals[2];

		B2.position = v2.position;
		B2.normal = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		B2.faceNormal1 = currentFaceNormal;
		B2.faceNormal2 = adjFaceNormals[2];

		C2 = A2;
		C2.normal = v0.normal;

		D2 = B2;
		D2.normal = v2.normal;

		*edgeVertices = A2; ++edgeVertices;
		*edgeVertices = B2; ++edgeVertices;
		*edgeVertices = C2; ++edgeVertices;
		*edgeVertices = D2; ++edgeVertices;
	}

	_vb->Unlock();
	mesh->UnlockVertexBuffer();
	mesh->UnlockIndexBuffer();
}



// 메쉬의 외곽선을 구함.
void SilhouetteEdges::genEdgeIndices(ID3DXMesh* mesh)
{
	DWORD numEdges = mesh->GetNumFaces() * 3;
	// 삼각형은 3개의 외곽선을 가지므로 각 면은 3개의 외곽선을 가짐.

	_numFaces = numEdges * 2;
	_device->CreateIndexBuffer(
		numEdges * 6 * sizeof(WORD), // 2 triangles per edge
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_ib,
		0);

	WORD* indices = 0;

	_ib->Lock(0, 0, (void**)&indices, 0);

	// 0        1
	// *--------*
	// |  edge  |
	// *--------*
	// 2        3

	for (UINT i = 0; i < numEdges; i++)
	{
		// Six indices to define the triangles of the edge,
		// so every edge we skip six entries in the
		// index buffer.  Four vertices to define the edge,
		// so every edge we skip four entries in the
		// vertex buffer.
		indices[i * 6] = i * 4 + 0;
		indices[i * 6 + 1] = i * 4 + 1;
		indices[i * 6 + 2] = i * 4 + 2;
		indices[i * 6 + 3] = i * 4 + 1;
		indices[i * 6 + 4] = i * 4 + 3;
		indices[i * 6 + 5] = i * 4 + 2;
	}

	_ib->Unlock();
}


void SilhouetteEdges::render()
{
	_device->SetVertexDeclaration(_decl);
	_device->SetStreamSource(0, _vb, 0, sizeof(EdgeVertex));
	_device->SetIndices(_ib);
	_device->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST, 0, 0, _numVerts, 0, _numFaces);
}

