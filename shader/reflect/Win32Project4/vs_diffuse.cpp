//////////////////////////////////////////////////////////////////////////////////////////////////
// File: vs_diffuse.cpp   
//////////////////////////////////////////////////////////////////////////////////////////////////

#include "d3dUtility.h"
//
// Globals
//

IDirect3DDevice9* Device = 0;

const int Width = 640;
const int Height = 480;

IDirect3DVertexShader9* DiffuseShader = 0;
// ���� ���̴��� ����ϱ� ���� �������̽�.
ID3DXConstantTable* DiffuseConstTable = 0;
// ���� ���̺� �����ϱ� ���� �������̽�.

ID3DXMesh* Teapot = 0;

// �����ϴ� �������� �̸��� ������ �ڵ� ����.
D3DXHANDLE ViewMatrixHandle = 0;
D3DXHANDLE ViewProjMatrixHandle = 0;
D3DXHANDLE AmbientMtrlHandle = 0;
D3DXHANDLE DiffuseMtrlHandle = 0;
D3DXHANDLE LightDirHandle = 0;

D3DXMATRIX Proj;

//
// Framework functions
//
bool Setup()
{
	HRESULT hr = 0;
	//
	// Create geometry:
	//
	D3DXCreateTeapot(Device, &Teapot, 0);
	// ������ �޽� ����.
	//
	// Compile shader
	//
	ID3DXBuffer* shader = 0;
	// ���̴��� ������ ����.
	ID3DXBuffer* errorBuffer = 0;
	// ���̴� ������ ������ ����.
	// ���̴��� �������Ѵ�.
	hr = D3DXCompileShaderFromFileA(
		"diffuse.txt",        // ���ϸ�
		0,                    // �������μ��� ������
		0,                    // #include �ǻ� ��� ó��
		"Main",                // ������ �����ϴ� �Լ� ������
		"vs_1_1",            // ���̴� ����
		D3DXSHADER_DEBUG,    // ���̴� ������ �ɼ�
		&shader,            // ���̴��� ������ ����
		&errorBuffer,        // ���̴� ������ ������ ����
		&DiffuseConstTable);    // ������̺� ������

	if (errorBuffer)
	{
		::MessageBoxA(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
		d3d::Release<ID3DXBuffer*>(errorBuffer);
	}

	if (FAILED(hr))
	{
		::MessageBox(0, L"D3DXCompileShaderFromFile() - FAILED", 0, 0);
		return false;
	}

	//
	// Create shader
	//

	// ���� ���̴� ����(�������� ���̴��� �����Ѵ�).
	hr = Device->CreateVertexShader(
		(DWORD*)shader->GetBufferPointer(),        // ���̴� ���� ������
		&DiffuseShader);                        // ������ ���̴� ������

	if (FAILED(hr))
	{
		::MessageBox(0, L"CreateVertexShader - FAILED", 0, 0);
		return false;
	}

	d3d::Release<ID3DXBuffer*>(shader);
	// �����Ͽ� ����� ���̴��� ���������� �����Ѵ�.

	//
	// Get Handles
	//

	// �� �̸��� �ش��ϴ� ���̴� ������ �ڵ��� ��´�.
	// �� �������� ���̴��� ���ǵǾ� �ִ�.
	// ���̴��� Ư�� ������ �����ϱ� ���ؼ��� ���� ������ �����ϴ� ����� �ʿ��ϸ�,
	// �̸� ���� D3DXHANDLE�� �̿�ȴ�.
	ViewMatrixHandle = DiffuseConstTable->GetConstantByName(0, "ViewMatrix");
	ViewProjMatrixHandle = DiffuseConstTable->GetConstantByName(0, "ViewProjMatrix");
	AmbientMtrlHandle = DiffuseConstTable->GetConstantByName(0, "AmbientMtrl");
	DiffuseMtrlHandle = DiffuseConstTable->GetConstantByName(0, "DiffuseMtrl");
	LightDirHandle = DiffuseConstTable->GetConstantByName(0, "LightDirection");
	//
	// Set shader constants:
	//
	// ������ ������ �����ϰ�,
	// ������ ���͸� LightDirHandle�� �����Ѵ�.
	D3DXVECTOR4 directionToLight(-0.57f, 0.57f, -0.57f, 0.0f);
	DiffuseConstTable->SetVector(Device, LightDirHandle, &directionToLight);

	// ������ ���ݻ籤 ����.
	D3DXVECTOR4 ambientMtrl(0.0f, 0.0f, 1.0f, 1.0f);
	D3DXVECTOR4 diffuseMtrl(0.0f, 0.0f, 1.0f, 1.0f);

	// ������ ���ݻ籤�� �� �ڵ鿡 �����ϰ�, �ʱ�ȭ�� �Ѵ�.
	DiffuseConstTable->SetVector(Device, AmbientMtrlHandle, &ambientMtrl);
	DiffuseConstTable->SetVector(Device, DiffuseMtrlHandle, &diffuseMtrl);
	DiffuseConstTable->SetDefaults(Device);

	// ���� ��� ���.
	D3DXMatrixPerspectiveFovLH(
		&Proj, D3DX_PI * 0.25f,
		(float)Width / (float)Height, 1.0f, 1000.0f);
	// ���Ϲ��� ������, y���⿡���� �þ�, ���μ��� ����,
	// ����� �� ����� z��, �� �� ����� z��.

	return true;
}

void Cleanup()
{
	d3d::Release<ID3DXMesh*>(Teapot);
	d3d::Release<IDirect3DVertexShader9*>(DiffuseShader);
	d3d::Release<ID3DXConstantTable*>(DiffuseConstTable);
}

bool Display(float timeDelta)
{
	if (Device)
	{
		//
		// Update the scene: Allow user to rotate around scene.
		//

		static float angle = (3.0f * D3DX_PI) / 2.0f;
		static float height = 3.0f;

		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			angle -= 0.5f * timeDelta;

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			angle += 0.5f * timeDelta;

		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			height += 5.0f * timeDelta;

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			height -= 5.0f * timeDelta;

		D3DXVECTOR3 position(cosf(angle) * 7.0f, height, sinf(angle) * 7.0f);
		// ī�޶� ��ġ.
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		// ������ ��ġ.
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		// �� ����
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);
		// �� ��� ����.
		DiffuseConstTable->SetMatrix(Device, ViewMatrixHandle, &V);
		// �� ����� ViewMatrixHandle�� �����Ѵ�.
		// ���յ� ��� ���� �����,
		// ViewProjMatrixHandle��Ŀ� ����.
		D3DXMATRIX ViewProj = V * Proj;
		DiffuseConstTable->SetMatrix(Device, ViewProjMatrixHandle, &ViewProj);
		//
		// Render
		//

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		Device->BeginScene();
		Device->SetVertexShader(DiffuseShader);
		// ���� ���̴��� ����(Ȱ��ȭ).
		Teapot->DrawSubset(0);
		// �����ڸ� �׸���.
		Device->EndScene();
		Device->Present(0, 0, 0, 0);
	}
	return true;
}


//
// WndProc
//

LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			::DestroyWindow(hwnd);

		break;
	}
	return ::DefWindowProc(hwnd, msg, wParam, lParam);
}

//
// WinMain
//
int WINAPI WinMain(HINSTANCE hinstance,
	HINSTANCE prevInstance,
	PSTR cmdLine,
	int showCmd)
{
	if (!d3d::InitD3D(hinstance,
		Width, Height, true, D3DDEVTYPE_HAL, &Device))
	{
		::MessageBox(0, L"InitD3D() - FAILED", 0, 0);
		return 0;
	}

	if (!Setup())
	{
		::MessageBox(0, L"Setup() - FAILED", 0, 0);
		return 0;
	}

	d3d::EnterMsgLoop(Display);

	Cleanup();

	Device->Release();

	return 0;
}