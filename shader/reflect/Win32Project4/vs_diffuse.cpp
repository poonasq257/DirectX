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
// 정점 셰이더를 사용하기 위한 인터페이스.
ID3DXConstantTable* DiffuseConstTable = 0;
// 정수 테이블에 접근하기 위한 인터페이스.

ID3DXMesh* Teapot = 0;

// 참조하는 변수들의 이름을 가지는 핸들 선언.
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
	// 주전자 메쉬 생성.
	//
	// Compile shader
	//
	ID3DXBuffer* shader = 0;
	// 셰이더를 저장할 버퍼.
	ID3DXBuffer* errorBuffer = 0;
	// 셰이더 에러를 저장할 버퍼.
	// 셰이더를 컴파일한다.
	hr = D3DXCompileShaderFromFileA(
		"diffuse.txt",        // 파일명
		0,                    // 프리프로세서 포인터
		0,                    // #include 의사 명령 처리
		"Main",                // 실행을 시작하는 함수 포인터
		"vs_1_1",            // 셰이더 버전
		D3DXSHADER_DEBUG,    // 셰이더 컴파일 옵션
		&shader,            // 셰이더를 저장할 버퍼
		&errorBuffer,        // 셰이더 에러를 저장할 버퍼
		&DiffuseConstTable);    // 상수테이블 포인터

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

	// 정점 셰이더 생성(컴파일한 셰이더를 저장한다).
	hr = Device->CreateVertexShader(
		(DWORD*)shader->GetBufferPointer(),        // 셰이더 버퍼 포인터
		&DiffuseShader);                        // 저장할 셰이더 포인터

	if (FAILED(hr))
	{
		::MessageBox(0, L"CreateVertexShader - FAILED", 0, 0);
		return false;
	}

	d3d::Release<ID3DXBuffer*>(shader);
	// 컴파일에 사용한 셰이더는 저장했으니 해제한다.

	//
	// Get Handles
	//

	// 각 이름에 해당하는 셰이더 변수의 핸들을 얻는다.
	// 저 변수들은 셰이더에 정의되어 있다.
	// 셰이더의 특정 변수를 지정하기 위해서는 먼저 변수를 참조하는 방법이 필요하며,
	// 이를 위해 D3DXHANDLE이 이용된다.
	ViewMatrixHandle = DiffuseConstTable->GetConstantByName(0, "ViewMatrix");
	ViewProjMatrixHandle = DiffuseConstTable->GetConstantByName(0, "ViewProjMatrix");
	AmbientMtrlHandle = DiffuseConstTable->GetConstantByName(0, "AmbientMtrl");
	DiffuseMtrlHandle = DiffuseConstTable->GetConstantByName(0, "DiffuseMtrl");
	LightDirHandle = DiffuseConstTable->GetConstantByName(0, "LightDirection");
	//
	// Set shader constants:
	//
	// 조명의 방향을 지정하고,
	// 지정한 벡터를 LightDirHandle에 지정한다.
	D3DXVECTOR4 directionToLight(-0.57f, 0.57f, -0.57f, 0.0f);
	DiffuseConstTable->SetVector(Device, LightDirHandle, &directionToLight);

	// 재질과 난반사광 지정.
	D3DXVECTOR4 ambientMtrl(0.0f, 0.0f, 1.0f, 1.0f);
	D3DXVECTOR4 diffuseMtrl(0.0f, 0.0f, 1.0f, 1.0f);

	// 재질과 난반사광을 각 핸들에 지정하고, 초기화를 한다.
	DiffuseConstTable->SetVector(Device, AmbientMtrlHandle, &ambientMtrl);
	DiffuseConstTable->SetVector(Device, DiffuseMtrlHandle, &diffuseMtrl);
	DiffuseConstTable->SetDefaults(Device);

	// 투영 행렬 계산.
	D3DXMatrixPerspectiveFovLH(
		&Proj, D3DX_PI * 0.25f,
		(float)Width / (float)Height, 1.0f, 1000.0f);
	// 리턴받을 포인터, y방향에서의 시야, 가로세로 비율,
	// 가까운 뷰 평면의 z값, 먼 뷰 평면의 z값.

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
		// 카메라 위치.
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		// 주전자 위치.
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		// 업 벡터
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);
		// 뷰 행렬 생성.
		DiffuseConstTable->SetMatrix(Device, ViewMatrixHandle, &V);
		// 뷰 행렬을 ViewMatrixHandle에 지정한다.
		// 결합된 뷰와 투영 행렬을,
		// ViewProjMatrixHandle행렬에 지정.
		D3DXMATRIX ViewProj = V * Proj;
		DiffuseConstTable->SetMatrix(Device, ViewProjMatrixHandle, &ViewProj);
		//
		// Render
		//

		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		Device->BeginScene();
		Device->SetVertexShader(DiffuseShader);
		// 정점 셰이더를 설정(활성화).
		Teapot->DrawSubset(0);
		// 주전자를 그린다.
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