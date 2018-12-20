#include "Camera.h"

Camera::Camera() : position(0, 0, 0), rotation(0, 0, 0) {}

Camera::Camera(const Camera& other) {}

Camera::~Camera() {}

void Camera::SetPosition(float x, float y, float z) 
{
	position.x = x;
	position.y = y;
	position.z = z;
}

void Camera::SetRotation(float x, float y, float z)
{
	rotation.x = x;
	rotation.y = y;
	rotation.z = z;
}

D3DXVECTOR3 Camera::GetPosition()
{
	return position;
}

D3DXVECTOR3 Camera::GetRotation()
{
	return rotation;
}

void Camera::Render()
{
	D3DXVECTOR3 up, worldPos, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	worldPos.x = position.x;
	worldPos.y = position.y;
	worldPos.z = position.z;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = rotation.x * 0.0174532925f;
	yaw = rotation.y * 0.0174532925f;
	roll = rotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = worldPos + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&viewMatrix, &worldPos, &lookAt, &up);
}

void Camera::GetViewMatrix(D3DXMATRIX& origin)
{
	origin = viewMatrix;
}