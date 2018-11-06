#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")

//direcx9 ���̺귯��
#include <d3dx9.h>



//-----------------------------------------------------------------------------//
// d3d
//-----------------------------------------------------------------------------//
namespace d3d
{
	// �����츦 �ʱ�ȭ�ϰ� Direct3D �ʱ�ȭ �ڵ带 �����ϴ� �Լ�
	bool InitD3D(
		HINSTANCE hInstance, // ���ø����̼� �ν��Ͻ�
		int width, int height, // �ĸ� ���� ũ��
		bool windowed, // ������(true) Ȥ�� ��üȭ��(false)
		D3DDEVTYPE deviceType, // HAL Ȥ�� REF
		IDirect3DDevice9** device); // ������� ��ġ

	//�޼��� ������ ���� : ����� ���� *���� �� �𸣰���.
	int EnterMsgLoop(bool (&ptr_display) (float timeDelta));

	//â ���ν���(����) : â�� �޴� �޼���(���)�� �����Ͽ� ����� �ڵ带 �����ϴ� ���
	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// COM �������̽��� �����ϰ� null�� �����ϴ� ���ø� �Լ�
	template<class T> void Release(T t)
	{
		if (t)
		{
			t->Release();
			t = 0;
		}
	}

	// ������� ��ü�� �����ϰ� �����͸� null�� �����ϴ� ���ø� �Լ�
	template<class T> void Delete(T t)
	{
		if (t)
		{
			t->Delete();
			t = 0;
		}
	}
}
