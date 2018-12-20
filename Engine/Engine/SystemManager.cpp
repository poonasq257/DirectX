#include "SystemManager.h"

SystemManager::SystemManager()
	: input(0), renderer(0), timer(0), position(0) {}

SystemManager::SystemManager(const SystemManager& other) {}

SystemManager::~SystemManager() {}

bool SystemManager::Initialize()
{
	int screenWidth, screenHeight;

	// Initialize the width and height of the screen to zero before sending the variables into the function.
	screenWidth = 0;
	screenHeight = 0;

	// Initialize the windows api.
	InitializeWindows(screenWidth, screenHeight);

	// Create the input object.  This object will be used to handle reading the keyboard input from the user.
	input = new InputManager;
	if (!input) return false;

	// Initialize the input object.
	if(!input->Initialize(hinstance, hwnd, screenWidth, screenHeight))
	{
		MessageBox(hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Create the graphics object.  This object will handle rendering all the graphics for this application.
	renderer = new Renderer;
	if (!renderer) return false;

	// Initialize the graphics object.
	if (!renderer->Initialize(screenWidth, screenHeight, hwnd)) return false;
	
	// Create the timer object.
	timer = new Timer;
	if (!timer) return false;

	// Initialize the timer object.
	if(!timer->Initialize())
	{
		MessageBox(hwnd, L"Could not initialize the timer object.", L"Error", MB_OK);
		return false;
	}

	// Initialize the position object.
	position = new Position;
	if (!position) return false;

	// Set the initial position of the viewer.
	position->SetPosition(0.0f, 1.5f, -11.0f);

	return true;
}


void SystemManager::Shutdown()
{
	// Release the position object.
	if (position)
	{
		delete position;
		position = 0;
	}
	// Release the timer object.
	if (timer)
	{
		delete timer;
		timer = 0;
	}
	// Release the graphics object.
	if(renderer)
	{
		renderer->Shutdown();
		delete renderer;
		renderer = 0;
	}

	// Release the input object.
	if(input)
	{
		input->Shutdown();
		delete input;
		input = 0;
	}

	// Shutdown the window.
	ShutdownWindows();
}


void SystemManager::Run()
{
	MSG msg;
	bool done = false;

	// Initialize the message structure.
	ZeroMemory(&msg, sizeof(MSG));
	
	// Loop until there is a quit message from the window or the user.
	while(!done)
	{
		// Handle the windows messages.
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// If windows signals to end the application then exit out.
		if(msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// Otherwise do the frame processing.
			if(!Frame())
			{
				done = true;
			}
		}
	}
}

bool SystemManager::Frame()
{
	bool keyDown;
	float positionX, positionY, positionZ;

	// Update the system stats.
	timer->Frame();

	// Do the input frame processing.
	if (!input->Frame()) return false;

	// Check if the user pressed escape and wants to exit the application.
	if (input->IsEscapePressed() == true) return false;

	// Set the frame time for calculating the updated position.
	position->SetFrameTime(timer->GetTime());

	// Handle the input.
	keyDown = input->IsLeftPressed();
	position->MoveLeft(keyDown);

	keyDown = input->IsRightPressed();
	position->MoveRight(keyDown);

	keyDown = input->IsUpPressed();
	position->MoveUp(keyDown);

	keyDown = input->IsDownPressed();
	position->MoveDown(keyDown);

	// Get the view point position/rotation.
	position->GetPosition(positionX, positionY, positionZ);

	// Do the frame processing for the graphics object.
	if (!renderer->Frame(positionX, positionY, positionZ)) return false;

	return true;
}


LRESULT CALLBACK SystemManager::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}


void SystemManager::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;
	// Get an external pointer to this object.	
	ApplicationHandle = this;

	// Get the instance of this application.
	hinstance = GetModuleHandle(NULL);

	// Give the application a name.
	applicationName = L"Engine";

	// Setup the windows class with default settings.
	wc.style         = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc   = WndProc;
	wc.cbClsExtra    = 0;
	wc.cbWndExtra    = 0;
	wc.hInstance     = hinstance;
	wc.hIcon		 = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm       = wc.hIcon;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = applicationName;
	wc.cbSize        = sizeof(WNDCLASSEX);
	
	// Register the window class.
	RegisterClassEx(&wc);

	// Determine the resolution of the clients desktop screen.
	screenWidth  = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Setup the screen settings depending on whether it is running in full screen or in windowed mode.
	if(FULL_SCREEN)
	{
		// If full screen set the screen to maximum size of the users desktop and 32bit.
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize       = sizeof(dmScreenSettings);
		dmScreenSettings.dmPelsWidth  = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		dmScreenSettings.dmBitsPerPel = 32;			
		dmScreenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Change the display settings to full screen.
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// Set the position of the window to the top left corner.
		posX = 0;
		posY = 0;
	}
	else
	{
		// If windowed then set it to 800x600 resolution.
		screenWidth  = 800;
		screenHeight = 600;

		// Place the window in the middle of the screen.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth)  / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// Create the window with the screen settings and get the handle to it.
	hwnd = CreateWindowEx(WS_EX_APPWINDOW, applicationName, applicationName, 
						    WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
						    posX, posY, screenWidth, screenHeight, NULL, NULL, hinstance, NULL);

	// Bring the window up on the screen and set it as main focus.
	ShowWindow(hwnd, SW_SHOW);
	SetForegroundWindow(hwnd);
	SetFocus(hwnd);

	// Hide the mouse cursor.
	ShowCursor(false);
}


void SystemManager::ShutdownWindows()
{
	// Show the mouse cursor.
	ShowCursor(true);

	// Fix the display settings if leaving full screen mode.
	if(FULL_SCREEN) ChangeDisplaySettings(NULL, 0);

	// Remove the window.
	DestroyWindow(hwnd);
	hwnd = NULL;

	// Remove the application instance.
	UnregisterClass(applicationName, hinstance);
	hinstance = NULL;

	// Release the pointer to this class.
	ApplicationHandle = NULL;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch(umessage)
	{
		// Check if the window is being destroyed.
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// Check if the window is being closed.
		case WM_CLOSE:
		{
			PostQuitMessage(0);		
			return 0;
		}

		// All other messages pass to the message handler in the system class.
		default:
		{
			return ApplicationHandle->MessageHandler(hwnd, umessage, wparam, lparam);
		}
	}
}