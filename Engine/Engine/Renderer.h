#ifndef _RENDERER_H_
#define _RENDERER_H_

#include "D3DFramework.h"
#include "Camera.h"
#include "Model.h"
#include "Shader.h"

const bool FULL_SCREEN = true;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;

class Renderer
{
private:
	D3DFramework* d3d;
	Camera* camera;
	Shader* shader;
	Model *floorModel, *originModel, *billboardModel;
private:
	bool Render();
public:
	Renderer();
	Renderer(const Renderer& other);
	~Renderer();
public:
	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(float, float, float);
};

#endif