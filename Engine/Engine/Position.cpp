#include "Position.h"

Position::Position() 
	: positionX(0.0f), positionY(0.0f), positionZ(0.0f),
	rotationX(0.0f), rotationY(0.0f), rotationZ(0.0f),
	frameTime(0.0f), leftSpeed(0.0f), rightSpeed(0.0f),
	upSpeed(0.0f), downSpeed(0.0f) {} 

Position::Position(const Position& other) {}

Position::~Position() {}

void Position::SetPosition(float x, float y, float z)
{
	positionX = x;
	positionY = y;
	positionZ = z;
}

void Position::SetRotation(float x, float y, float z)
{
	rotationX = x;
	rotationY = y;
	rotationZ = z;
}

void Position::GetPosition(float& x, float& y, float& z)
{
	x = positionX;
	y = positionY;
	z = positionZ;
}

void Position::GetRotation(float& x, float& y, float& z)
{
	x = rotationX;
	y = rotationY;
	z = rotationZ;
}

void Position::SetFrameTime(float time)
{
	frameTime = time;
}

void Position::MoveLeft(bool keydown)
{
	// Update the forward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		leftSpeed += frameTime * 0.001f;
		if(leftSpeed > (frameTime * 0.03f)) leftSpeed = frameTime * 0.03f;
	}
	else
	{
		leftSpeed -= frameTime * 0.0007f;
		if(leftSpeed < 0.0f) leftSpeed = 0.0f;
	}

	// Convert degrees to radians.
	float radians = rotationY * 0.0174532925f;

	// Update the position.
	positionX -= cosf(radians) * leftSpeed;
	positionZ -= sinf(radians) * leftSpeed;
}


void Position::MoveRight(bool keydown)
{
	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if(keydown)
	{
		rightSpeed += frameTime * 0.001f;
		if(rightSpeed > (frameTime * 0.03f)) rightSpeed = frameTime * 0.03f;
	}
	else
	{
		rightSpeed -= frameTime * 0.0007f;
		if(rightSpeed < 0.0f) rightSpeed = 0.0f;
	}

	// Convert degrees to radians.
	float radians = rotationY * 0.0174532925f;

	// Update the position.
	positionX += cosf(radians) * rightSpeed;
	positionZ += sinf(radians) * rightSpeed;
}

void Position::MoveUp(bool keydown)
{
	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		upSpeed += frameTime * 0.001f;
		if (upSpeed > (frameTime * 0.03f)) upSpeed = frameTime * 0.03f;
	}
	else
	{
		upSpeed -= frameTime * 0.0007f;
		if (upSpeed < 0.0f) upSpeed = 0.0f;
	}

	// Convert degrees to radians.
	float radians = rotationY * 0.0174532925f;

	// Update the position.
	positionY += cosf(radians) * upSpeed;
	positionZ += sinf(radians) * upSpeed;
}

void Position::MoveDown(bool keydown)
{
	// Update the backward speed movement based on the frame time and whether the user is holding the key down or not.
	if (keydown)
	{
		downSpeed += frameTime * 0.001f;
		if (downSpeed > (frameTime * 0.03f)) downSpeed = frameTime * 0.03f;
	}
	else
	{
		downSpeed -= frameTime * 0.0007f;
		if (downSpeed < 0.0f) downSpeed = 0.0f;
	}

	// Convert degrees to radians.
	float radians = rotationY * 0.0174532925f;

	// Update the position.
	positionY -= cosf(radians) * downSpeed;
	positionZ -= sinf(radians) * downSpeed;
}