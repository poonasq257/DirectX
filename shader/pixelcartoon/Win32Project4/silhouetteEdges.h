//////////////////////////////////////////////////////////////////////////////////////////////////
// File: silhouetteEdges.h   
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __silhouetteEdgesH__
#define __silhouetteEdgesH__
#include "d3dUtility.h"

// 외곽선 버텍스.
struct EdgeVertex
{
	D3DXVECTOR3 position;            // 위치
	D3DXVECTOR3 normal;                // 법선
	D3DXVECTOR3 faceNormal1;        // 면 법선
	D3DXVECTOR3 faceNormal2;        // 면 법선
};

// 메쉬 버텍스.
struct MeshVertex
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	static const DWORD FVF;
};

// 클래스.
class SilhouetteEdges
{
public:
	SilhouetteEdges(
		IDirect3DDevice9* device,
		ID3DXMesh* mesh,
		ID3DXBuffer* adjBuffer);

	~SilhouetteEdges();

	void render();

private:
	IDirect3DDevice9*            _device;    // 디바이스.
	IDirect3DVertexBuffer9*      _vb;        // 버텍스 버퍼.
	IDirect3DIndexBuffer9*       _ib;        // 인덱스 버퍼.
	IDirect3DVertexDeclaration9* _decl;        // 정점 셰이더 선언을 캡슐화하는 인터페이스.

	UINT _numVerts;        // 버텍스 수.
	UINT _numFaces;        // 면 수.

private:
	// 정점 셰이더 선언을 생성.
	bool createVertexDeclaration();
	// 면의 법선 벡터를 계산.
	void getFaceNormal(
		ID3DXMesh* mesh,
		DWORD faceIndex,
		D3DXVECTOR3* faceNormal);

	// 면 경계 테스트(외곽선인지 테스트).
	void getFaceNormals(
		ID3DXMesh* mesh,
		ID3DXBuffer* adjBuffer,
		D3DXVECTOR3* currentFaceNormal,
		D3DXVECTOR3 adjFaceNormals[3],
		DWORD       faceIndex);

	// 버텍스가 외곽선에 있는지 검사.
	void genEdgeVertices(
		ID3DXMesh* mesh,
		ID3DXBuffer* adjBuffer);

	// 메쉬의 외곽선을 구함.
	void genEdgeIndices(
		ID3DXMesh* mesh);
};
#endif // __silhouetteEdgesH__