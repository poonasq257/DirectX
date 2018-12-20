#include "PCH.h"

Renderer::Renderer() 
	: d3d(nullptr), camera(nullptr), shader(nullptr),
	floorModel(nullptr), originModel(nullptr), billboardModel(nullptr) {}

Renderer::Renderer(const Renderer& other) {}

Renderer::~Renderer() {}

bool Renderer::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	// Create the Direct3D object.
	d3d = new D3DFramework;
	if (!d3d) return false;

	// Initialize the Direct3D object.
	if(!d3d->Initialize(screenWidth, screenHeight,
		VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR))
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	camera = new Camera;
	if (!camera) return false;

	// Set the initial position of the camera.
	camera->SetPosition(0.0f, 0.0f, -10.0f);
	
	// Create the texture shader object.
	shader = new Shader;
	if (!shader) return false;

	// Initialize the texture shader object.
	if(!shader->Initialize(d3d->GetDevice(), hwnd))
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the floor model object.
	floorModel = new Model;
	if(!floorModel) return false;

	// Initialize the floor model object.
	if(!floorModel->Initialize(d3d->GetDevice(), "../Engine/data/floor.txt", L"../Engine/data/grid01.dds"))
	{
		MessageBox(hwnd, L"Could not initialize the floor model object.", L"Error", MB_OK);
		return false;
	}

	// Create the billboard model object.
	billboardModel = new Model;
	if (!billboardModel) return false;

	// Initialize the billboard model object.
	if(!billboardModel->Initialize(d3d->GetDevice(), "../Engine/data/square.txt", L"../Engine/data/seafloor.dds"))
	{
		MessageBox(hwnd, L"Could not initialize the billboard model object.", L"Error", MB_OK);
		return false;
	}

	originModel = new Model;
	if (!originModel) return false;

	// Initialize the billboard model object.
	if (!originModel->Initialize(d3d->GetDevice(), "../Engine/data/square.txt", L"../Engine/data/seafloor.dds"))
	{
		MessageBox(hwnd, L"Could not initialize the billboard model object.", L"Error", MB_OK);
		return false;
	}

	return true;
}


void Renderer::Shutdown()
{
	// Release the billboard model object.
	if(billboardModel)
	{
		billboardModel->Shutdown();
		delete billboardModel;
		billboardModel = 0;
	}


	if (originModel)
	{
		originModel->Shutdown();
		delete originModel;
		originModel = 0;
	}
	// Release the floor model object.
	if(floorModel)
	{
		floorModel->Shutdown();
		delete floorModel;
		floorModel = 0;
	}

	// Release the texture shader object.
	if(shader)
	{
		shader->Shutdown();
		delete shader;
		shader = 0;
	}

	// Release the camera object.
	if(camera)
	{
		delete camera;
		camera = 0;
	}

	// Release the D3D object.
	if(d3d)
	{
		d3d->Shutdown();
		delete d3d;
		d3d = 0;
	}
}


bool Renderer::Frame(float positionX, float positionY, float positionZ)
{
	// Update the position of the camera.
	camera->SetPosition(positionX, positionY, positionZ);

	// Render the graphics scene.
	if (!Render()) return false;

	return true;
}


bool Renderer::Render()
{
	D3DXMATRIX billMatrix, viewMatrix, projectionMatrix, translateMatrix;
	D3DXMATRIX originMatrix;
	D3DXVECTOR3 cameraPosition, billPosition, originPosition;
	double angleX, angleY;
	float rotationX, rotationY;

	// Clear the buffers to begin the scene.
	d3d->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	camera->GetViewMatrix(viewMatrix);
	d3d->GetWorldMatrix(billMatrix);
	d3d->GetWorldMatrix(originMatrix);
	d3d->GetProjectionMatrix(projectionMatrix);

	// Put the floor model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	floorModel->Render(d3d->GetDeviceContext());

	// Render the floor model using the texture shader.
	if (!shader->Render(d3d->GetDeviceContext(), floorModel->GetIndexCount(),
		billMatrix, viewMatrix, projectionMatrix, floorModel->GetTexture())) return false;

	// Get the position of the camera.
	cameraPosition = camera->GetPosition();

	// Set the position of the billboard model.
	billPosition.x = 1.5f;
	billPosition.y = 1.5f;
	billPosition.z = 0.0f;

	originPosition.x = -1.5f;
	originPosition.y = 1.5f;
	originPosition.z = 0.0f;

	// Calculate the rotation that needs to be applied to the billboard model to face the current camera position using the arc tangent function.
	angleX = -atan2(billPosition.y - cameraPosition.y, billPosition.z - cameraPosition.z) * (180.0 / D3DX_PI);
	angleY = atan2(billPosition.x - cameraPosition.x, billPosition.z - cameraPosition.z) * (180.0 / D3DX_PI);

	// Convert rotation into radians.
	rotationX = (float)angleX * 0.0174532925f;
	rotationY = (float)angleY * 0.0174532925f;

	// Setup the rotation the billboard at the origin using the world matrix.
	D3DXMATRIX matRotX, matRotY;
	D3DXMatrixIdentity(&matRotX);
	D3DXMatrixIdentity(&matRotY);

	D3DXMatrixRotationX(&matRotX, rotationX);
	D3DXMatrixRotationY(&matRotY, rotationY);

	D3DXMatrixMultiply(&matRotY, &matRotY, &matRotX);
	D3DXMatrixMultiply(&billMatrix, &billMatrix, &matRotY);

	// Setup the translation matrix from the billboard model.
	D3DXMatrixTranslation(&translateMatrix, billPosition.x, billPosition.y, billPosition.z);
	D3DXMatrixTranslation(&originMatrix, originPosition.x, originPosition.y, originPosition.z);

	// Finally combine the rotation and translation matrices to create the final world matrix for the billboard model.
	D3DXMatrixMultiply(&billMatrix, &billMatrix, &translateMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	billboardModel->Render(d3d->GetDeviceContext());
	// Render the model using the texture shader.
	if (!shader->Render(d3d->GetDeviceContext(), billboardModel->GetIndexCount(),
		billMatrix, viewMatrix, projectionMatrix, billboardModel->GetTexture())) return false;

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	originModel->Render(d3d->GetDeviceContext());
	// Render the model using the texture shader.
		if (!shader->Render(d3d->GetDeviceContext(), originModel->GetIndexCount(),
			originMatrix, viewMatrix, projectionMatrix, originModel->GetTexture())) return false;

	// Present the rendered scene to the screen.
	d3d->EndScene();

	return true;
}