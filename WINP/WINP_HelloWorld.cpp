
//������ ���̺귯�� ����. ������ ���α׷��ֿ� �ʿ��� ����ü, �Լ� ���� �� ����
#include <windows.h>

// HWND�� �������� �ν���Ʈ ����. HWND�� â�Ƿ��� �ڵ��� �ǹ�. �׷� �ڵ��� ����?
//�����ϸ� �Ƹ� �ּҰ��� �ǹ� ���� �� ���ο� â�� ȣ���Ҷ� ���ο� â�� �ڵ��� �ʿ����. �ּҰ��� �ƴϴ��� �� â�� ���� �ĺ� �����ϰ� �ϴ� ������ �ڵ��̶�� �ϴµ�.
HWND MainWindowHandle = 0;

// ���� ���ø����̼� â�� ����� �ʱ�ȭ(������ 1, ���н� 0 ��ȯ)
bool InitWindowsApp(HINSTANCE instanceHandle, int show);

// ���ø����̼��� �޼��� ������ ĸ��ȭ *�̰� �� ĸ��ȭ��??
int  Run();

//����â�� ���� ���ν���(â�� Ư��)
LRESULT CALLBACK WndProc(HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);

// ���������� �����Լ�. C�� ġ�� int main()����
int WINAPI WinMain(HINSTANCE hInstance, // ���� ���ø����̼��� �ν���Ʈ �ڵ�
	HINSTANCE hPrevInstance, //  32��Ʈ������ ������� ����(0�� ����)
	PSTR      pCmdLine, // ���α׷� ���࿡ ���� ��ɾ� ���� ���� ���ڿ� *���Ҹ���?
	int       nShowCmd) // â�� �����ִ� ��� : �ִ�ȭ�� �ּ�ȭ�� ��...
{

	if (!InitWindowsApp(hInstance, nShowCmd)) //���� ���ø����̼� â ����
	{
		::MessageBox(0, "Init - Failed", "Error", MB_OK); // �޼��� �ڽ� ȣ��
		return 0;
	}

	return Run(); // Run ����
}

bool InitWindowsApp(HINSTANCE instanceHandle, int show)
{

	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc; // â ���ν��� �Լ�
	wc.cbClsExtra = 0; // �ΰ��� �޸� ����1
	wc.cbWndExtra = 0; // �ΰ��� �޸� ����2
	wc.hInstance = instanceHandle; // ���ø����̼� �ν������� �ڵ� ����
	//wc.hIcon = ::LoadIcon(0, IDI_APPLICATION); // ����â�� �������� �ڵ� ����
	wc.hIcon = ::LoadIcon(0, IDI_ASTERISK);
	//wc.hCursor = ::LoadCursor(0, IDC_ARROW); // Ŀ���� �ڵ� ����
	wc.hCursor = ::LoadCursor(0, IDC_WAIT);
	wc.hbrBackground =  // â�� ��� �÷� ����
		//static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
		static_cast<HBRUSH>(::GetStockObject(DKGRAY_BRUSH));
	wc.lpszMenuName = 0; // â�� �޴� ����
	wc.lpszClassName = "Hello!"; // â�� Ŭ���� ����ü �̸� ����

	if (!::RegisterClass(&wc)) // â������ �����쿡 ���
	{
		::MessageBox(0, "RegisterClass - Failed", 0, 0);
		return false;
	}


	MainWindowHandle = ::CreateWindow( 
		"Hello!", // ������� â�� ������ ���Ե� ����ü �̸� wc.lpszClassName �� ����
		"Hello!!!", // ������ â�� ���鶧 ǥ�õǴ� �̸�
		WS_OVERLAPPEDWINDOW, // â�� ��Ÿ�� ����
		CW_USEDEFAULT, //  â �»�� X��ǥ ��ġ ����
		CW_USEDEFAULT, // â ���� Y��ǥ ��ġ ����
		CW_USEDEFAULT, // �ʼ� ���� â�� �ʺ�
		CW_USEDEFAULT, // �ȼ����� â�� ����
		0, // ���� â�� �θ� �� â�� �ڵ�(�� â�� �θ� ���⿡ 0�� ����)
		0, // �޷���� �ڵ�(�޴��� ���⿡ 0 ����)
		instanceHandle, // â�� ����Ǵ�(�ڽ�) ���ø���Ű�� �ڵ�
		0); //����� ������ �����ͷ��� ������

	if (MainWindowHandle == 0) 
	{
		::MessageBox(0, "CreateWindow - Failed", 0, 0);
		return false;
	}

	::ShowWindow(MainWindowHandle, show); // â ǥ��
	::UpdateWindow(MainWindowHandle); // â ����

	return true;
}

int Run()
{
	MSG msg; // �޼��� ����
	::ZeroMemory(&msg, sizeof(MSG)); // �޼��� �ʱ�ȭ

	while (::GetMessage(&msg, 0, 0, 0)) // �޼����� �޴µ� �����ϸ�
	{
		::TranslateMessage(&msg); // ���ڿ� �޼������� ����Ű �޽��� ��ȯ
		::DispatchMessage(&msg); // ���������� ������ â ���ν����� �޼��� ����
	}

	return msg.wParam;
}

//â ���ν���(����) : â�� �޴� �޼���(���)�� �����Ͽ� ����� �ڵ带 �����ϴ� ���
LRESULT CALLBACK WndProc(HWND   windowHandle,  // CALLBACK : �޵��찡 �ܺ������� ȣ���� �ݹ� �Լ� ����, �޼����� ���� â�� �ڵ�
	UINT   msg, // �޼��� �ĺ��� ���� �ĺ���
	WPARAM wParam, // �޼����� ���� �ΰ��� ����1
	LPARAM lParam) // �޼����� ���� �ΰ��� ����2
{
	switch (msg)
	{

	case WM_LBUTTONDOWN: // ���콺 ���� Ŭ��
		::MessageBox(0, "Hello, World", "Hello", MB_OK); // â�� �ڵ�, â�� ����, â�� �̸�, �޽��� ������ ��Ÿ��
		return 0;


	case WM_KEYDOWN: // ȭ��ǥ �Ʒ� ��ư
		if (wParam == VK_ESCAPE)
			::DestroyWindow(MainWindowHandle);
		return 0;

	case WM_DESTROY: // â �ı�(����)
		::PostQuitMessage(0); // �޼��� ���� ����
		return 0;
	}

	return ::DefWindowProc(windowHandle, // default â ���ν���(â �ִ�ȭ, �ּ�ȭ, ũ�� ����, �ݱ� ���� ����)
		msg,
		wParam,
		lParam);
}
