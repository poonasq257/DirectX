#include "d3dUtility.h"


IDirect3DDevice9* Device = 0;

int Width = 1024;
int Height = 768;

ID3DXMesh* mesh = 0;
//�߰� ������Ʈ �޽�
ID3DXMesh* Objects[5] = { 0, 0, 0, 0, 0 };


// ���� �����̽������� �޽� ��ġ�� �����ϴ� �࿭
D3DXMATRIX ObjWorldMatrices[6];

//ť�� ������ ���� ���ؽ��� �ε��� ����
IDirect3DVertexBuffer9* VB = 0;
IDirect3DIndexBuffer9*  IB = 0;

//�ﰢ���� ���ؽ� ����
IDirect3DVertexBuffer9* Triangle = 0; // vertex buffer to store
									  // our triangle data.


struct Vertex
{
	Vertex() {}
	Vertex(float x, float y, float z)
	{
		_x = x;  _y = y;  _z = z;
	}
	float _x, _y, _z;
	static const DWORD FVF;
};
const DWORD Vertex::FVF = D3DFVF_XYZ;


//-----------------------------------------------------------------------------//
// Setup :�ڿ� �Ҵ�, ��ġ�� Ư�� Ȯ��, ���ø����̼� ������ ���� ��� ���� ������ �����ϱ� ���� ���� ���� �۾� ����
//-----------------------------------------------------------------------------//
bool Setup()
{
	//ID3DXMesh* mesh = 0;
	D3DXCreateTeapot(Device, &mesh, 0);


	Device->CreateVertexBuffer(
		8 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF,
		D3DPOOL_MANAGED,
		&VB,
		0);

	Device->CreateIndexBuffer(
		39 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&IB,
		0);


	Vertex* vertices;
	VB->Lock(0, 0, (void**)&vertices, 0);

	// vertices of a unit cube
	vertices[0] = Vertex(-1.0f, -1.0f, -1.0f);
	vertices[1] = Vertex(-1.0f, 1.0f, -1.0f);
	vertices[2] = Vertex(1.0f, 1.0f, -1.0f);
	vertices[3] = Vertex(1.0f, -1.0f, -1.0f);
	vertices[4] = Vertex(-1.0f, -1.0f, 1.0f);
	vertices[5] = Vertex(-1.0f, 1.0f, 1.0f);
	vertices[6] = Vertex(1.0f, 1.0f, 1.0f);
	vertices[7] = Vertex(1.0f, -1.0f, 1.0f);
	
	//�ﰢ���� ���� ���ؽ�
	vertices[0] = Vertex(-1.0f, 0.0f, 2.0f);
	vertices[1] = Vertex(0.0f, 5.0f, 20.0f);
	vertices[2] = Vertex(1.0f, 0.0f, 12.0f);

	VB->Unlock();

	WORD* indices = 0;
	IB->Lock(0, 0, (void**)&indices, 0);

	// front side
	indices[0] = 0; indices[1] = 1; indices[2] = 2;
	indices[3] = 0; indices[4] = 2; indices[5] = 3;

	// back side
	indices[6] = 4; indices[7] = 6; indices[8] = 5;
	indices[9] = 4; indices[10] = 7; indices[11] = 6;

	// left side
	indices[12] = 4; indices[13] = 5; indices[14] = 1;
	indices[15] = 4; indices[16] = 1; indices[17] = 0;

	// right side
	indices[18] = 3; indices[19] = 2; indices[20] = 6;
	indices[21] = 3; indices[22] = 6; indices[23] = 7;

	// top
	indices[24] = 1; indices[25] = 5; indices[26] = 6;
	indices[27] = 1; indices[28] = 6; indices[29] = 2;

	// bottom
	indices[30] = 4; indices[31] = 0; indices[32] = 3;
	indices[33] = 4; indices[34] = 3; indices[35] = 7;

	IB->Unlock();

	D3DXCreateTeapot(
		Device,
		&Objects[0],
		0);

	D3DXCreateBox(
		Device,
		2.0f, // width
		2.0f, // height
		2.0f, // depth
		&Objects[1],
		0);

	D3DXCreateCylinder(
		Device,
		1.0f, // radius at negative z end
		1.0f, // radius at positive z end
		3.0f, // length of cylinder
		10,   // slices
		10,   // stacks
		&Objects[2],
		0);

	D3DXCreateTorus(
		Device,
		1.0f, // inner radius
		3.0f, // outer radius
		10,   // sides
		10,   // rings
		&Objects[3],
		0);

	D3DXCreateSphere(
		Device,
		1.0f, // radius
		10,   // slices
		10,   // stacks
		&Objects[4],
		0);

	// ���� �����̽� �Ҵ�
	D3DXMatrixTranslation(&ObjWorldMatrices[0], 0.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&ObjWorldMatrices[1], -5.0f, 0.0f, 5.0f);
	D3DXMatrixTranslation(&ObjWorldMatrices[2], 5.0f, 0.0f, 5.0f);
	D3DXMatrixTranslation(&ObjWorldMatrices[3], -5.0f, 0.0f, -5.0f);
	D3DXMatrixTranslation(&ObjWorldMatrices[4], 5.0f, 0.0f, -5.0f);
	//D3DXMatrixTranslation(&ObjWorldMatrices[6], 3.0f, -4.0f, 0.0f);

	// ----------------------------------------- //
	// ī�޶� �ʱ�ȭ(��ġ�� ���� ����)
	// ----------------------------------------- //

	D3DXVECTOR3 position(0.0f, 0.0f, 5.0f); // ���� ���� ī�޶� ��ġ
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f); // ���� ���� ī�޶� ���� ����
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f); // ������ �� ����
	D3DXMATRIX V; // ��� ����� ���� ������
	D3DXMatrixLookAtLH(&V, &position, &target, &up); // 2.3.3 �� �����̽� ��ȯ ��� (��� ����� ���� ������, ���� ���� ī�޶� ��ġ, ���� ���� ī�޶� ���� ����, ������ �� ����)
	Device->SetTransform(D3DTS_VIEW, &V);

	// ----------------------------------------- //
	// ���� ����� �����Ѵ� : n������ n-1 �������� ��ȯ
	// ----------------------------------------- //

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.6f, (float) Width / (float) Height, 1.0f, 1000.0f);
	// ���� ���, �þ߰��� ���� ����(����), ��Ȳ�� = �ʺ� / ����, ������ �������� �Ÿ�, �� �������� �Ÿ�
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	// ----------------------------------------- //
	// ���� ���¸� �����Ѵ�
	// ----------------------------------------- //

	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);


	/*
	//-------------------------------------------//
	//����Ʈ : ��ũ���ӿ��� D3D�� ����ϴ� ȭ���� ũ��-�������� ���� ��� ������ ũ�⿡ ���缭 ���
	//-------------------------------------------//
	D3DVIEWPORT9 vport = {0,0,640,480,0,1};
	Device->SetViewport(&vport);
	*/

	return true;
}

//-----------------------------------------------------------------------------//
// Display - ȭ�鿡 ǥ���ϴ� �κ� :��� ����� �ڵ� ����, ������ ���� �Ͼ�� �۾� ����
//-----------------------------------------------------------------------------//
bool Display(float timeDelta)// timeDelta : �� ������ ��� �ð� - �ʴ� �����Ӱ� �ִϸ��̼� ����ȭ�� �̿�.
{
	if (Device)
	{
		// ----------------------------------------- //
		// �Թ�ü�� ȸ����Ų��
		// ----------------------------------------- //
		/*
		D3DXMATRIX rx, ry;

		// x������ 45�� ȸ����Ų��
		D3DXMatrixRotationX(&rx, 3.14f / 4.0f);

		// �� �����Ӵ� y ȸ���� �����õf��
		static float y = 0.0f;
		D3DXMatrixRotationY(&ry, y);
		y += timeDelta;

		// ������ 2 * PI�� �̸��� 0���� �ʱ�ȭ�Ѵ�
		if (y >= 6.28f)
			y = 0.0f;

		// ȸ���� �����Ѵ�
		D3DXMATRIX p = rx * ry;

		Device->SetTransform(D3DTS_WORLD, &p);
		*/
		

		// ----------------------------------------- //
		// ����� �׷�����
		// ----------------------------------------- //

		// �ĸ� ���۸� �Ұ��ϰ� ���� ����, ���ٽ� ���۸� ���� ���(0xFFFFFFFF)�� 1.0���� �ʱ�ȭ�Ѵ�
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

		Device->BeginScene(); // ��� �׸��� ����!
		
		// ť�� 
		//Device->SetTransform(D3DTS_WORLD, &ObjWorldMatrices[6]);
		Device->SetStreamSource(0, VB, 0, sizeof(Vertex));
		Device->SetIndices(IB);
		Device->SetFVF(Vertex::FVF);
		//Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
		for (int i = 0; i < 5; i++)
		{
			// ���ý����̽� -> ���� �����̽� ��ȯ
			Device->SetTransform(D3DTS_WORLD, &ObjWorldMatrices[i]);

			// Draw the object using the previously set world matrix.
			Objects[i]->DrawSubset(0);
		}
		

		//mesh->DrawSubset(0); // �����ڸ� �׸���

		Device->EndScene(); // ��� �׸��� ��!

		// �ĸ� ���۸� �����Ѵ�
		Device->Present(0, 0, 0, 0);
	}

	return true;
}

//-----------------------------------------------------------------------------//
// Cleanup : Setup�Լ����� �Ҵ�� �ڿ��� �����ϴ� �۾� ����
//-----------------------------------------------------------------------------//
void Cleanup()
{

	if (Device != NULL)
		Device->Release();
	for (int i = 0; i < 5; i++)
		d3d::Release<ID3DXMesh*>(Objects[i]);

	d3d::Release<IDirect3DVertexBuffer9*>(VB);
	d3d::Release<IDirect3DIndexBuffer9*>(IB);

	mesh->Release();
	mesh = 0;
}

//-----------------------------------------------------------------------------//
// WndProc : ������ â���� �޼���(���)�Է½� �����ϴ� �۾�
//-----------------------------------------------------------------------------//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) // ESC Ű�� ������ ���α׷��� �����Ѵ�
			DestroyWindow(hwnd);

		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------//
// WinMain : ������ ���α׷����� main
//-----------------------------------------------------------------------------//
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, LPSTR cmdLine, int showCmd)
{
	if (!d3d::InitD3D(hInstance, Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		MessageBox(0, TEXT("InitD3D() - FAILED"), 0, 0);
		return 0;
	}

	if (!Setup())
	{
		MessageBox(0, TEXT("Setup() - FAILED"), 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display); // �޽��� ����

	// �޽��� ������ ���������� ���� �۾�
	Cleanup();
	Device->Release();
	
	return 0;
}
