#ifndef _POSITION_H_
#define _POSITION_H_

#include <math.h>

class Position
{
private:
	float positionX, positionY, positionZ;
	float rotationX, rotationY, rotationZ;

	float frameTime;

	float leftSpeed, rightSpeed, upSpeed, downSpeed;
public:
	Position();
	Position(const Position& ohter);
	~Position();
public:
	void SetPosition(float x, float y, float z);
	void SetRotation(float x, float y, float z);

	void GetPosition(float& x, float& y, float& z);
	void GetRotation(float& x, float& y, float& z);

	void SetFrameTime(float time);

	void MoveLeft(bool keydown);
	void MoveRight(bool keydown);
	void MoveUp(bool keydown);
	void MoveDown(bool keydown);
};

#endif