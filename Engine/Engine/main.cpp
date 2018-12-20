#include "SystemManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemManager* system;
	// Create the system object.
	system = new SystemManager;
	if (!system) return 0;

	// Initialize and run the system object.
	if(system->Initialize()) system->Run();

	// Shutdown and release the system object.
	system->Shutdown();
	delete system;
	system = 0;

	return 0;
}