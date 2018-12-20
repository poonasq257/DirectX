#ifndef _SYSTEMMANAGER_H_
#define _SYSTEMMANAGER_H_
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

#include "InputManager.h"
#include "Renderer.h"
#include "Timer.h"
#include "Position.h"

class SystemManager
{
private:
	LPCWSTR applicationName;
	HINSTANCE hinstance;
	HWND hwnd;

	InputManager* input;
	Renderer* renderer;
	Timer* timer;
	Position* position;
private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();
public:
	SystemManager();
	SystemManager(const SystemManager& other);
	~SystemManager();
public:
	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);
};

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static SystemManager* ApplicationHandle = 0;

#endif