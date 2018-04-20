#include "stdafx.h"
#include "cFrame.h"

bool	cFrame::isTimerStopped = true;
float	cFrame::timeElapsed = 0.0f;

cFrame::cFrame() 
	:ticksPerSecond(0)
	, currentTime(0)
	, lastTime(0)
	, lastFPSUpdate(0)
	, fpsUpdateInterval(0)
	, frameCount(0)
	, runningTime(0)
	, framePerSecond(0)
{
	QueryPerformanceFrequency((LARGE_INTEGER *)&ticksPerSecond);
	fpsUpdateInterval = ticksPerSecond >> 1;
}

cFrame::~cFrame()
{

}

void cFrame::Update()
{
	if (isTimerStopped) return;

	//1. 현재시간을 가져와 시간 간격 및 진행 시간을 계산한다.
	QueryPerformanceCounter((LARGE_INTEGER *)&currentTime);
	timeElapsed = (float)(currentTime - lastTime) / (float)ticksPerSecond;
	runningTime += timeElapsed;

	//2. FPS Update
	frameCount++;
	if (currentTime - lastFPSUpdate >= fpsUpdateInterval)
	{
		float tempCurrentcFrame = (float)currentTime / (float)ticksPerSecond;
		float tempLastcFrame = (float)lastFPSUpdate / (float)ticksPerSecond;
		framePerSecond = (float)frameCount / (tempCurrentcFrame - tempLastcFrame);

		lastFPSUpdate = (INT64)currentTime;
		frameCount = 0;
	}

	lastTime = currentTime;
}

void cFrame::Print()
{

}

void cFrame::Start()
{
	if (!isTimerStopped)
		assert(false && "이미 카운트가 진행되었습니다!");

	QueryPerformanceCounter((LARGE_INTEGER *)&lastTime);
	isTimerStopped = false;
}

void cFrame::Stop()
{
	if (isTimerStopped)
		assert(false && "이미 카운트가 멈춰있습니다!");

	INT64 stopcFrame = 0;
	QueryPerformanceCounter((LARGE_INTEGER *)&stopcFrame);
	runningTime += (float)(stopcFrame - lastTime) / (float)ticksPerSecond;
	isTimerStopped = true;
}