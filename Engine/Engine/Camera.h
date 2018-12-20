#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <d3dx10math.h>

class Camera
{
private:
	D3DXVECTOR3 position;
	D3DXVECTOR3 rotation;
	D3DXMATRIX viewMatrix;
public:
	Camera();
	Camera(const Camera& other);
	~Camera();
public:
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();

	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	void Render();
	void GetViewMatrix(D3DXMATRIX&);
};

#endif