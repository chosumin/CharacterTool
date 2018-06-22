#include "stdafx.h"
#include "cMath.h"

const float cMath::PI = 3.14159265f;

const int cMath::IntMinValue = -2147483648;
const int cMath::IntMaxValue = 2147483647;
const float cMath::FloatMinValue = -3.402823E+38f;
const float cMath::FloatMaxValue = 3.402823E+38f;

float cMath::ToRadian(float degree)
{
	return degree * PI / 180.0f;
}

float cMath::ToDegree(float radian)
{
	return radian * 180.0f / PI;
}

float cMath::Random(float r1, float r2)
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = r2 - r1;
	float val = random * diff;
	
	return r1 + val;
}

float cMath::Clamp(float value, float min, float max)
{
	value = value > max ? max : value;
	value = value < min ? min : value;

	return value;
}

D3DXVECTOR3 cMath::Mul(D3DXVECTOR3 & a, D3DXVECTOR3 & b)
{
	return{ a.x * b.x, a.y * b.y, a.z * b.z };
}

int cMath::Random(int r1, int r2)
{
	return (int)(rand() % (r2 - r1 + 1)) + r1;
}