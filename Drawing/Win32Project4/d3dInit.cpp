#include "d3dUtility.h"


struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw;
	DWORD color;       
};
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)

// 삼각형 제작용
LPDIRECT3D9             g_pD3D = NULL; // Used to create the D3DDevice
LPDIRECT3DDEVICE9       g_pd3dDevice = NULL; // Our rendering device
LPDIRECT3DVERTEXBUFFER9 g_pVB = NULL; // Buffer to hold Vertices
 // 여기 까지

// 용책 DRAW
IDirect3DVertexBuffer9* VB = 0;
//IDirect3DIndexBuffer9* IB = 0;




IDirect3DDevice9* Device = 0;

int Width = 1024;
int Height = 768;

ID3DXMesh* mesh = 0;

//-----------------------------------------------------------------------------//
// 삼각형용 VERTEX
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
// Setup :자원 할당, 장치의 특성 확인, 어플리케이션 상태의 설정 등과 같이 예제를 실행하기 위한 사전 준지 작업 수행
//-----------------------------------------------------------------------------//
bool Setup()
{
	//ID3DXMesh* mesh = 0;
	D3DXCreateTeapot(Device, &mesh, 0);

	// ----------------------------------------- //
	// 카메라 초기화(위치와 방향 조정)
	// ----------------------------------------- //

	D3DXVECTOR3 position(0.0f, 0.0f, 5.0f); // 월드 내의 카메라 위치
	D3DXVECTOR3 target(0.0f, 0.0f, 0.0f); // 월드 내의 카메라가 보는 지점
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f); // 월드의 업 벡터
	D3DXMATRIX V; // 결과 행렬을 받을 포인터
	D3DXMatrixLookAtLH(&V, &position, &target, &up); // 2.3.3 뷰 스페이스 변환 행렬 (결과 행렬을 받을 포인터, 월드 내의 카메라 위치, 월드 내의 카메라가 보는 지점, 월드의 업 벡터)
	Device->SetTransform(D3DTS_VIEW, &V);

	// ----------------------------------------- //
	// 투영 행렬을 지정한다 : n차원을 n-1 차원으로 변환
	// ----------------------------------------- //

	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.6f, (float) Width / (float) Height, 1.0f, 1000.0f);
	// 투영 행렬, 시야각의 수직 영역(라디안), 종황비 = 너비 / 높이, 가가운 평면까지의 거리, 먼 평면까지의 거리
	Device->SetTransform(D3DTS_PROJECTION, &proj);

	// ----------------------------------------- //
	// 렌더 상태를 지정한다
	// ----------------------------------------- //

	Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);


	/*
	//-------------------------------------------//
	//뷰포트 : 스크린속에서 D3D를 출력하는 화면의 크기-설정하지 않을 경우 윈도우 크기에 맞춰서 출력
	//-------------------------------------------//
	D3DVIEWPORT9 vport = {0,0,640,480,0,1};
	Device->SetViewport(&vport);
	*/

	return true;
}

//-----------------------------------------------------------------------------//
// Display - 화면에 표시하는 부분 :모든 드로인 코드 구현, 프레임 마다 일어나는 작업 수행
//-----------------------------------------------------------------------------//
bool Display(float timeDelta)// timeDelta : 각 프레임 경과 시간 - 초당 프레임과 애니메이션 동기화에 이용.
{
	if (Device)
	{
		// ----------------------------------------- //
		// 입방체를 회전시킨다
		// ----------------------------------------- //
		/*
		D3DXMATRIX rx, ry;

		// x축으로 45도 회전시킨다
		D3DXMatrixRotationX(&rx, 3.14f / 4.0f);

		// 각 프레임당 y 회전을 증가시킼다
		static float y = 0.0f;
		D3DXMatrixRotationY(&ry, y);
		y += timeDelta;

		// 각도가 2 * PI에 이르면 0으로 초기화한다
		if (y >= 6.28f)
			y = 0.0f;

		// 회전을 결합한다
		D3DXMATRIX p = rx * ry;

		Device->SetTransform(D3DTS_WORLD, &p);
		*/

		// ----------------------------------------- //
		// 장면을 그려낸다
		// ----------------------------------------- //

		// 후면 버퍼를 소거하고 깊이 버퍼, 스텐실 버퍼를 각각 흰색(0xFFFFFFFF)과 1.0으로 초기화한다
		Device->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xFFFFFFFF, 1.0f, 0);

		Device->BeginScene(); // 장면 그리기 시작!

		// 삼각형 그리기
		Device->SetStreamSource(0, g_pVB, 0, sizeof(CUSTOMVERTEX));
		Device->SetFVF(D3DFVF_CUSTOMVERTEX);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);


		//mesh->DrawSubset(0); // 주전자를 그린다

		Device->EndScene(); // 장면 그리기 끝!

		// 후면 버퍼를 선언한다
		Device->Present(0, 0, 0, 0);
	}

	return true;
}

//-----------------------------------------------------------------------------//
// Cleanup : Setup함수에서 할당된 자원을 해제하는 작업 수행
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
// WndProc : 윈도우 창에서 메세지(명령)입력시 시행하는 작업
//-----------------------------------------------------------------------------//
LRESULT CALLBACK d3d::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE) // ESC 키를 누르면 프로그램을 종료한다
			DestroyWindow(hwnd);

		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}

//-----------------------------------------------------------------------------//
// WinMain : 윈도우 프로그래밍의 main
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

	d3d::EnterMsgLoop(Display); // 메시지 루프

	// 메시지 루프를 빠져나오면 종료 작업
	Cleanup();
	Device->Release();
	
	return 0;
}
