
#include "d3dUtility.h"

LPCTSTR lpszClass = TEXT("D3DSample - 04. Teapot"); // ������ Ÿ��Ʋ


bool d3d::InitD3D(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType, IDirect3DDevice9** device)
{
	//-------------------------------------------------------------------//
	// ���� ������ �ۼ� : ������ ���α׷��� �κ�(InitWindowsApp)
	//-------------------------------------------------------------------//

	// ȭ�� ũ�� ���� �ܿ��� Ư���� �ǵ帱 �κ��� ����.
	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)d3d::WndProc; 
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH); 
	// DKGRAY�� �����ص� �̰� â�� ���ϻ�. â���� ī�޶� �����ִ� ���彺���̽��� DKGRAY�� �ƴϱ⿡ ���� �ǹ� ����
	wc.lpszMenuName = 0;
	wc.lpszClassName = lpszClass;

	if (!RegisterClass(&wc)) 
	{
		MessageBox(0, TEXT("RegisterClass() - FAILED"), 0, 0);
		return false;
	}
		
	HWND hwnd = 0;
	hwnd = CreateWindow(lpszClass, lpszClass, WS_EX_TOPMOST | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 
		0, 0, width, height, 0, 0, hInstance, 0); 

	if (!hwnd)
	{
		MessageBox(0, TEXT("CreateWindow() - FAILED"), 0, 0);
		return false;
	}

	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);
	

	//-------------------------------------------------------------------//
	// Init D3D
	//-------------------------------------------------------------------//

	HRESULT hr = 0;

	// ----------------------------------------- //
	// 1. IDirect3D9 ��ü ����
	// ----------------------------------------- //

	IDirect3D9* d3d9 = 0;
    d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

    if (!d3d9)
	{
		MessageBox(0, TEXT("Direct3DCreate9() - FAILED"), 0, 0);
		return false;
	}

	// ----------------------------------------- //
	// 2. �ϵ���� ���ؽ� ���μ��� Ȯ��
	// ----------------------------------------- //

	// �⺻ ���÷��� ������� Ư������ D3DCAPS9 ����ü�� ä���

	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(
		D3DADAPTER_DEFAULT,	// �⺻ ���÷��� ����͸� ǥ��
		deviceType,	// ��ġ Ÿ���� ����(���� D3DDEVTYPE_HAL)
		&caps);	// D3DCAPS9 ����ü�� ����

	int vp = 0;

	// �ϵ���� ���ؽ� ���μ����� �̿��� �� �ִ°�
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING; // �ϵ���� ���ؽ� ���μ������� vp�� ����
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; // ����Ʈ���� ���ؽ� ���μ������� vp�� ����

	d3d9->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, deviceType, D3DFMT_A8R8G8B8, 
		false, D3DMULTISAMPLE_16_SAMPLES, NULL);

	// ----------------------------------------- //
	// 3. D3DPRESENT_PARAMETERS ����ü ä���
	// ----------------------------------------- //

	D3DPRESENT_PARAMETERS d3dpp;

	d3dpp.BackBufferWidth = width; // �ĸ� ���� �ʺ�
	d3dpp.BackBufferHeight = height; // �ĸ� ���� ����
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8; // �ĸ� ������ �ȼ� ����(32��Ʈ �ȼ� ����)
	d3dpp.BackBufferCount = 1; // �̿��� �ĸ� ������ ��
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE; // ��Ƽ ���ø� Ÿ��
	d3dpp.MultiSampleQuality = 0; // ��Ƽ ���ø� ����
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD; // �ø��� ü���� ���۰� ��ȯ�Ǵ� ����� ����
	d3dpp.hDeviceWindow = hwnd; // ������ �ڵ�
	d3dpp.Windowed = windowed; // â(true), ��üȭ��(false) ���
	d3dpp.EnableAutoDepthStencil = true; // Direct3D�� �ڵ����� ���� ����, ���ٽ� ���۸� ����� �����Ѵ�(true)
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8; // ���� ����, ���ٽ� ���� ����
	d3dpp.Flags = 0; // �� ���� �ΰ����� Ư����
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT; // ȭ�� �ֻ���
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE; // 

	// ----------------------------------------- //
	// 4. ��ġ ����
	// ----------------------------------------- //

	hr = d3d9->CreateDevice(
		D3DADAPTER_DEFAULT, // ���� �׷��� ��ġ
		deviceType, // ��ġ Ÿ��
		hwnd, // ��ġ�� ����� ������
		vp, // ���ؽ� ���μ��� Ÿ��
	    &d3dpp, // �ÿ� ����
	    device); // ������ ��ġ

	// ��ġ ������ �������� ���
	if (FAILED(hr))
	{
		// 16-bit ���� ���۷� �ٽ��ѹ� �õ��Ѵ�
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		
		hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, deviceType, hwnd, vp, &d3dpp, device);

		if (FAILED(hr))
		{
			d3d9->Release(); // done with d3d9 object
			MessageBox(0, TEXT("CreateDevice() - FAILED"), 0, 0);
			return false;
		}
	}

	d3d9->Release(); // done with d3d9 object

	return true;
}

//-----------------------------------------------------------------------------//
// EnterMsgLoop - �޽��� ����
//-----------------------------------------------------------------------------//
int d3d::EnterMsgLoop(bool (&ptr_display) (float timeDelta))
{
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	static float lastTime = (float) timeGetTime();

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			float currTime = (float) timeGetTime();
			float timeDelta = (currTime - lastTime) * 0.001f;

			ptr_display(timeDelta); // ���÷��� �Լ��� ȣ���Ѵ�

			lastTime = currTime;
		}
	}

	return msg.wParam;
}
