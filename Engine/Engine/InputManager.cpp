#include "InputManager.h"

InputManager::InputManager() : directInput(0), keyboard(0), mouse(0) {}

InputManager::InputManager(const InputManager& other) {}

InputManager::~InputManager() {}

bool InputManager::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	// Store the screen size which will be used for positioning the mouse cursor.
	screenWidth = this->screenWidth;
	screenHeight = this->screenHeight;

	// Initialize the location of the mouse on the screen.
	mouseX = 0;
	mouseY = 0;

	// Initialize the main direct input interface.
	if (FAILED(DirectInput8Create(hinstance, DIRECTINPUT_VERSION,
		IID_IDirectInput8, (void**)&directInput, NULL))) return false;

	// Initialize the direct input interface for the keyboard.
	if (FAILED(directInput->CreateDevice(
		GUID_SysKeyboard, &keyboard, NULL))) return false;

	// Set the data format.  In this case since it is a keyboard we can use the predefined data format.
	if (FAILED(keyboard->SetDataFormat(&c_dfDIKeyboard))) return false;

	// Set the cooperative level of the keyboard to not share with other programs.
	if (FAILED(keyboard->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE))) return false;

	// Now acquire the keyboard.
	if (FAILED(keyboard->Acquire()))  return false;

	// Initialize the direct input interface for the mouse.
	if (FAILED(directInput->CreateDevice(
		GUID_SysMouse, &mouse, NULL))) return false;

	// Set the data format for the mouse using the pre-defined mouse data format.
	if (FAILED(mouse->SetDataFormat(&c_dfDIMouse))) return false;

	// Set the cooperative level of the mouse to share with other programs.
	if (FAILED(mouse->SetCooperativeLevel(
		hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) return false;

	// Acquire the mouse.
	if (FAILED(mouse->Acquire())) return false;

	return true;
}


void InputManager::Shutdown()
{
	// Release the mouse.
	if (mouse)
	{
		mouse->Unacquire();
		mouse->Release();
		mouse = 0;
	}

	// Release the keyboard.
	if (keyboard)
	{
		keyboard->Unacquire();
		keyboard->Release();
		keyboard = 0;
	}

	// Release the main interface to direct input.
	if (directInput)
	{
		directInput->Release();
		directInput = 0;
	}
}


bool InputManager::Frame()
{
	// Read the current state of the keyboard.
	if (!ReadKeyboard())
	{
		return false;
	}

	// Read the current state of the mouse.
	if (!ReadMouse())
	{
		return false;
	}

	// Process the changes in the mouse and keyboard.
	ProcessInput();

	return true;
}


bool InputManager::ReadKeyboard()
{
	// Read the keyboard device.
	HRESULT result = keyboard->GetDeviceState(sizeof(keyboardState), (LPVOID)&keyboardState);
	if (FAILED(result))
	{
		// If the keyboard lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}


bool InputManager::ReadMouse()
{
	// Read the mouse device.
	HRESULT result = mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&mouseState);
	if (FAILED(result))
	{
		// If the mouse lost focus or was not acquired then try to get control back.
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}


void InputManager::ProcessInput()
{
	// Update the location of the mouse cursor based on the change of the mouse location during the frame.
	mouseX += mouseState.lX;
	mouseY += mouseState.lY;

	// Ensure the mouse location doesn't exceed the screen width or height.
	if (mouseX < 0) mouseX = 0;
	if (mouseY < 0) mouseY = 0;

	if (mouseX > screenWidth) mouseX = screenWidth;
	if (mouseY > screenHeight) mouseY = screenHeight; 
}


bool InputManager::IsEscapePressed()
{
	// Do a bitwise and on the keyboard state to check if the escape key is currently being pressed.
	if (keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}


void InputManager::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = this->mouseX;
	mouseY = this->mouseY;
}


bool InputManager::IsLeftPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (keyboardState[DIK_LEFT] & 0x80)
	{
		return true;
	}

	return false;
}


bool InputManager::IsRightPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (keyboardState[DIK_RIGHT] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputManager::IsUpPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (keyboardState[DIK_UP] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputManager::IsDownPressed()
{
	// Do a bitwise and on the keyboard state to check if the key is currently being pressed.
	if (keyboardState[DIK_DOWN] & 0x80)
	{
		return true;
	}

	return false;
}