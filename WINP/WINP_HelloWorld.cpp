
//윈도우 라이브러리 선언. 윈도우 프로그래밍에 필요한 구조체, 함수 선언 등 가능
#include <windows.h>

// HWND형 전역변수 인스턴트 생성. HWND는 창의로의 핸들을 의미. 그럼 핸들은 무엇?
//추측하면 아마 주소값의 의미 예를 들어서 새로운 창을 호출할때 새로운 창의 핸들을 필요로함. 주소값이 아니더라도 그 창에 대해 식별 가능하게 하는 정보를 핸들이라고 하는듯.
HWND MainWindowHandle = 0;

// 메인 어플리케이션 창을 만들고 초기화(성공시 1, 실패시 0 반환)
bool InitWindowsApp(HINSTANCE instanceHandle, int show);

// 어플리케이션의 메세지 루프를 캡슐화 *이게 왜 캡슐화임??
int  Run();

//메인창에 대한 프로시저(창의 특성)
LRESULT CALLBACK WndProc(HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);

// 윈프에서의 메인함수. C로 치면 int main()역할
int WINAPI WinMain(HINSTANCE hInstance, // 현재 어플리케이션의 인스턴트 핸들
	HINSTANCE hPrevInstance, //  32비트에서는 사용하지 않음(0을 저장)
	PSTR      pCmdLine, // 프로그램 실행에 사용된 명령어 라인 인자 문자열 *뭔소리여?
	int       nShowCmd) // 창을 보여주는 방식 : 최대화면 최소화면 등...
{

	if (!InitWindowsApp(hInstance, nShowCmd)) //메인 어플리케이션 창 생성
	{
		::MessageBox(0, "Init - Failed", "Error", MB_OK); // 메세지 박스 호출
		return 0;
	}

	return Run(); // Run 실행
}

bool InitWindowsApp(HINSTANCE instanceHandle, int show)
{

	WNDCLASS wc;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc; // 창 프로시저 함수
	wc.cbClsExtra = 0; // 부가적 메모리 슬롯1
	wc.cbWndExtra = 0; // 부가적 메모리 슬롯2
	wc.hInstance = instanceHandle; // 어플리케이션 인스턴으의 핸들 지정
	//wc.hIcon = ::LoadIcon(0, IDI_APPLICATION); // 현재창의 아이콘의 핸들 지정
	wc.hIcon = ::LoadIcon(0, IDI_ASTERISK);
	//wc.hCursor = ::LoadCursor(0, IDC_ARROW); // 커서의 핸들 지정
	wc.hCursor = ::LoadCursor(0, IDC_WAIT);
	wc.hbrBackground =  // 창의 배경 컬러 지정
		//static_cast<HBRUSH>(::GetStockObject(WHITE_BRUSH));
		static_cast<HBRUSH>(::GetStockObject(DKGRAY_BRUSH));
	wc.lpszMenuName = 0; // 창의 메뉴 지정
	wc.lpszClassName = "Hello!"; // 창의 클래스 수조체 이름 지정

	if (!::RegisterClass(&wc)) // 창정보를 윈도우에 등록
	{
		::MessageBox(0, "RegisterClass - Failed", 0, 0);
		return false;
	}


	MainWindowHandle = ::CreateWindow( 
		"Hello!", // 만들려는 창의 정보가 포함된 구조체 이름 wc.lpszClassName 과 동일
		"Hello!!!", // 실제로 창을 만들때 표시되는 이름
		WS_OVERLAPPEDWINDOW, // 창의 스타일 정의
		CW_USEDEFAULT, //  창 좌상단 X좌표 위치 지정
		CW_USEDEFAULT, // 창 우상단 Y좌표 위치 지정
		CW_USEDEFAULT, // 필셀 단위 창의 너비
		CW_USEDEFAULT, // 픽셀단위 창의 높이
		0, // 현재 창의 부모가 될 창의 핸들(이 창은 부모가 없기에 0값 지정)
		0, // 메루오의 핸들(메뉴가 없기에 0 지정)
		instanceHandle, // 창과 연결되는(자식) 어플리게키션 핸들
		0); //사용자 지정의 데이터로의 포인터

	if (MainWindowHandle == 0) 
	{
		::MessageBox(0, "CreateWindow - Failed", 0, 0);
		return false;
	}

	::ShowWindow(MainWindowHandle, show); // 창 표시
	::UpdateWindow(MainWindowHandle); // 창 갱신

	return true;
}

int Run()
{
	MSG msg; // 메세지 선언
	::ZeroMemory(&msg, sizeof(MSG)); // 메세제 초기화

	while (::GetMessage(&msg, 0, 0, 0)) // 메세지를 받는데 성공하면
	{
		::TranslateMessage(&msg); // 문자열 메세지로의 가상키 메시지 변환
		::DispatchMessage(&msg); // 최종적으로 적절한 창 프로시저에 메세지 전달
	}

	return msg.wParam;
}

//창 프로시저(절차) : 창이 받는 메세지(명령)에 응답하여 실행될 코드를 포함하는 장소
LRESULT CALLBACK WndProc(HWND   windowHandle,  // CALLBACK : 왼도우가 외부적으로 호출할 콜백 함수 지정, 메세지를 받을 창의 핸들
	UINT   msg, // 메세지 식별을 위한 식별갑
	WPARAM wParam, // 메세지에 따른 부가적 정보1
	LPARAM lParam) // 메세지에 따른 부가적 정보2
{
	switch (msg)
	{

	case WM_LBUTTONDOWN: // 마우스 좌측 클릭
		::MessageBox(0, "Hello, World", "Hello", MB_OK); // 창의 핸들, 창의 내용, 창의 이름, 메시지 상자의 스타일
		return 0;


	case WM_KEYDOWN: // 화살표 아래 버튼
		if (wParam == VK_ESCAPE)
			::DestroyWindow(MainWindowHandle);
		return 0;

	case WM_DESTROY: // 창 파괴(종료)
		::PostQuitMessage(0); // 메세지 루프 종료
		return 0;
	}

	return ::DefWindowProc(windowHandle, // default 창 프로시저(창 최대화, 최소화, 크기 변경, 닫기 등이 구현)
		msg,
		wParam,
		lParam);
}
