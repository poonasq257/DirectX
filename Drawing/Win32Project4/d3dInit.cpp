#include "d3dUtility.h"


IDirect3DDevice9* Device = 0;

int Width = 1024;
int Height = 768;

ID3DXMesh* mesh = 0;
//추가 오브젝트 메쉬
ID3DXMesh* Objects[5] = { 0, 0, 0, 0, 0 };


// 월드 스페이스에서의 메쉬 위치를 관리하는 행열
D3DXMATRIX ObjWorldMatrices[6];

//큐브 생성을 위한 버텍스와 인덱스 버퍼
IDirect3DVertexBuffer9* VB = 0;
IDirect3DIndexBuffer9*  IB = 0;

//삼각형용 버텍스 버퍼
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
// Setup :자원 할당, 장치의 특성 확인, 어플리케이션 상태의 설정 등과 같이 예제를 실행하기 위한 사전 준지 작업 수행
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
	
	//삼각형을 위한 버텍스
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

	// 월드 스페이스 할당
	D3DXMatrixTranslation(&ObjWorldMatrices[0], 0.0f, 0.0f, 0.0f);
	D3DXMatrixTranslation(&ObjWorldMatrices[1], -5.0f, 0.0f, 5.0f);
	D3DXMatrixTranslation(&ObjWorldMatrices[2], 5.0f, 0.0f, 5.0f);
	D3DXMatrixTranslation(&ObjWorldMatrices[3], -5.0f, 0.0f, -5.0f);
	D3DXMatrixTranslation(&ObjWorldMatrices[4], 5.0f, 0.0f, -5.0f);
	//D3DXMatrixTranslation(&ObjWorldMatrices[6], 3.0f, -4.0f, 0.0f);

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
		
		// 큐브 
		//Device->SetTransform(D3DTS_WORLD, &ObjWorldMatrices[6]);
		Device->SetStreamSource(0, VB, 0, sizeof(Vertex));
		Device->SetIndices(IB);
		Device->SetFVF(Vertex::FVF);
		//Device->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);
		Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);
		for (int i = 0; i < 5; i++)
		{
			// 로컬스페이스 -> 월드 스페이스 변환
			Device->SetTransform(D3DTS_WORLD, &ObjWorldMatrices[i]);

			// Draw the object using the previously set world matrix.
			Objects[i]->DrawSubset(0);
		}
		

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
