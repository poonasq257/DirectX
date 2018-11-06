#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "winmm.lib")

//direcx9 라이브러리
#include <d3dx9.h>



//-----------------------------------------------------------------------------//
// d3d
//-----------------------------------------------------------------------------//
namespace d3d
{
	// 윈도우를 초기화하고 Direct3D 초기화 코드를 구현하는 함수
	bool InitD3D(
		HINSTANCE hInstance, // 애플리케이션 인스턴스
		int width, int height, // 후면 버퍼 크기
		bool windowed, // 윈도우(true) 혹은 전체화면(false)
		D3DDEVTYPE deviceType, // HAL 혹은 REF
		IDirect3DDevice9** device); // 만들어진 장치

	//메세지 루프를 포장 : 드로잉 수행 *아직 잘 모르겠음.
	int EnterMsgLoop(bool (&ptr_display) (float timeDelta));

	//창 프로시저(절차) : 창이 받는 메세지(명령)에 응답하여 실행될 코드를 포함하는 장소
	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	// COM 인터페이스를 해제하고 null로 지정하는 템플릿 함수
	template<class T> void Release(T t)
	{
		if (t)
		{
			t->Release();
			t = 0;
		}
	}

	// 저장고의 객체를 제거하고 포인터를 null로 지정하는 템플릿 함수
	template<class T> void Delete(T t)
	{
		if (t)
		{
			t->Delete();
			t = 0;
		}
	}
}
