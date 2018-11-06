#include "d3dUtility.h"


struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw;
	DWORD color;       
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

// �ﰢ�� ���ۿ�
LPDIRECT3D9             g_pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // Buffer to hold Vertices
 // ���� ����

// ��å DRAW
IDirect3DVertexBuffer9* VB = 0;
//IDirect3DIndexBuffer9* IB = 0;




IDirect3DDevice9* Device = 0;

int Width = 1024;
int Height = 768;

ID3DXMesh* mesh = 0;

//-----------------------------------------------------------------------------//
// �ﰢ���� VERTEX
//-----------------------------------------------------------------------------//
HRESULT InitVB()
{
	// Initialize three Vertices for rendering a triangle
	CUSTOMVERTEX Vertices[] =
	{
		{ 150.0f,  50.0f, 0.5f, 1.0f, 0xffff0000, }, // x, y, z, rhw, color
		{ 250.0f, 250.0f, 0.5f, 1.0f, 0xff00ff00, },
		{ 50.0f, 250.0f, 0.5f, 1.0f, 0xff00ffff, },
	};

	// Create the vertex buffer. Here we are allocating enough memory
	// (from the default pool) to hold all our 3 custom Vertices. We also
	// specify the FVF, so the vertex buffer knows what data it contains.
	if (FAILED(Device->CreateVertexBuffer(3 * sizeof(CUSTOMVERTEX),
		0, D3DFVF_CUSTOMVERTEX,
		D3DPOOL_DEFAULT, &g_pVB, NULL)))
	{
		return E_FAIL;
	}

	// Now we fill the vertex buffer. To do this, we need to Lock() the VB to
	// gain access to the Vertices. This mechanism is required becuase vertex
	// buffers may be in device memory.
	VOID* pVertices;
	if (FAILED(g_pVB->Lock(0, sizeof(Vertices), (void**)&pVertices, 0)))
		return E_FAIL;
	memcpy(pVertices, Vertices, sizeof(Vertices));
	g_pVB->Unlock();

	return S_OK;
}

//-----------------------------------------------------------------------------//
// Setup :�ڿ� �Ҵ�, ��ġ�� Ư�� Ȯ��, ���ø����̼� ������ ���� ��� ���� ������ �����ϱ� ���� ���� ���� �۾� ����
//-----------------------------------------------------------------------------//
bool Setup()
{
	//ID3DXMesh* mesh = 0;
	D3DXCreateTeapot(Device, &mesh, 0);

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

		// �ﰢ�� �׸���
		Device->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
		Device->SetFVF(D3DFVF_CUSTOMVERTEX);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);


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
	if (g_pVB != NULL)
		g_pVB->Release();

	if (Device != NULL)
		Device->Release();

	if (g_pD3D != NULL)
		g_pD3D->Release();

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
