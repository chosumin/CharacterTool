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

	//1. ����ð��� ������ �ð� ���� �� ���� �ð��� ����Ѵ�.
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
		assert(false && "�̹� ī��Ʈ�� ����Ǿ����ϴ�!");

	QueryPerformanceCounter((LARGE_INTEGER *)&lastTime);
	isTimerStopped = false;
}

void cFrame::Stop()
{
	if (isTimerStopped)
		assert(false && "�̹� ī��Ʈ�� �����ֽ��ϴ�!");

	INT64 stopcFrame = 0;
	QueryPerformanceCounter((LARGE_INTEGER *)&stopcFrame);
	runningTime += (float)(stopcFrame - lastTime) / (float)ticksPerSecond;
	isTimerStopped = true;
}