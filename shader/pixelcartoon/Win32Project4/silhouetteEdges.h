//////////////////////////////////////////////////////////////////////////////////////////////////
// File: silhouetteEdges.h   
//////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef __silhouetteEdgesH__
#define __silhouetteEdgesH__
#include "d3dUtility.h"

// �ܰ��� ���ؽ�.
struct EdgeVertex
{
	D3DXVECTOR3 position;            // ��ġ
	D3DXVECTOR3 normal;                // ����
	D3DXVECTOR3 faceNormal1;        // �� ����
	D3DXVECTOR3 faceNormal2;        // �� ����
};

// �޽� ���ؽ�.
struct MeshVertex
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	static const DWORD FVF;
};

// Ŭ����.
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
	IDirect3DDevice9*            _device;    // ����̽�.
	IDirect3DVertexBuffer9*      _vb;        // ���ؽ� ����.
	IDirect3DIndexBuffer9*       _ib;        // �ε��� ����.
	IDirect3DVertexDeclaration9* _decl;        // ���� ���̴� ������ ĸ��ȭ�ϴ� �������̽�.

	UINT _numVerts;        // ���ؽ� ��.
	UINT _numFaces;        // �� ��.

private:
	// ���� ���̴� ������ ����.
	bool createVertexDeclaration();
	// ���� ���� ���͸� ���.
	void getFaceNormal(
		ID3DXMesh* mesh,
		DWORD faceIndex,
		D3DXVECTOR3* faceNormal);

	// �� ��� �׽�Ʈ(�ܰ������� �׽�Ʈ).
	void getFaceNormals(
		ID3DXMesh* mesh,
		ID3DXBuffer* adjBuffer,
		D3DXVECTOR3* currentFaceNormal,
		D3DXVECTOR3 adjFaceNormals[3],
		DWORD       faceIndex);

	// ���ؽ��� �ܰ����� �ִ��� �˻�.
	void genEdgeVertices(
		ID3DXMesh* mesh,
		ID3DXBuffer* adjBuffer);

	// �޽��� �ܰ����� ����.
	void genEdgeIndices(
		ID3DXMesh* mesh);
};
#endif // __silhouetteEdgesH__