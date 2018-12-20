#ifndef _TIMER_H_
#define _TIMER_H_

#include <windows.h>

class Timer
{
private:
	INT64 frequency;
	float ticksPerMs;
	INT64 startTime;
	float frameTime;
public:
	Timer();
	Timer(const Timer& other);
	~Timer();
public:
	bool Initialize();
	void Frame();

	float GetTime();
};

#endif