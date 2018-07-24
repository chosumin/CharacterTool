#include "stdafx.h"
#include "cMath.h"

const float cMath::PI = 3.14159265f;
const D3DXMATRIX cMath::MATRIX_IDENTITY = 
{
	1,0,0,0,
	0,1,0,0,
	0,0,1,0,
	0,0,0,1
};
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

D3DXQUATERNION cMath::ToQuaternion(const D3DXVECTOR3 & angles)
{
	D3DXQUATERNION q;
	
	float cy = cosf(angles.y * RADIAN * 0.5f);
	float sy = sinf(angles.y * RADIAN  * 0.5f);
	float cr = cosf(angles.z * RADIAN * 0.5f);
	float sr = sinf(angles.z * RADIAN * 0.5f);
	float cp = cosf(angles.x * RADIAN * 0.5f);
	float sp = sinf(angles.x * RADIAN * 0.5f);

	q.w = cy * cr * cp + sy * sr * sp;
	q.x = cy * sr * cp - sy * cr * sp;
	q.y = cy * cr * sp + sy * sr * cp;
	q.z = sy * cr * cp - cy * sr * sp;

	return q;
}

D3DXVECTOR3 cMath::ToEulerAngles(const D3DXQUATERNION & q)
{
	//hack : 이 식이 맞나?
	D3DXVECTOR3 angles;

	// roll (x-axis rotation)
	float sinr = +2.0f * (q.w * q.x + q.y * q.z);
	float cosr = +1.0f - 2.0f * (q.x * q.x + q.y * q.y);
	angles.z = atan2(sinr, cosr) / RADIAN;

	// pitch (y-axis rotation)
	float sinp = +2.0f * (q.w * q.y - q.z * q.x);
	if (fabs(sinp) >= 1)
		angles.x = copysign(PI / 2, sinp) / RADIAN; // use 90 degrees if out of range
	else
		angles.x = asin(sinp) / RADIAN;

	// yaw (z-axis rotation)
	float siny = +2.0f * (q.w * q.z + q.x * q.y);
	float cosy = +1.0f - 2.0f * (q.y * q.y + q.z * q.z);
	angles.y = atan2(siny, cosy) / RADIAN;

	return angles;
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

D3DXVECTOR3 cMath::Clamp(const D3DXVECTOR3 & value, const D3DXVECTOR3 & min, const D3DXVECTOR3 & max)
{
	float x = Clamp(value.x, min.x, max.x);
	float y = Clamp(value.y, min.y, max.y);
	float z = Clamp(value.z, min.z, max.z);

	return{ x,y,z };
}

D3DXVECTOR3 cMath::Mul(D3DXVECTOR3 & a, D3DXVECTOR3 & b)
{
	return{ a.x * b.x, a.y * b.y, a.z * b.z };
}

float cMath::DistanceSquared(const D3DXVECTOR3 & value1, const D3DXVECTOR3 & value2)
{
	float x = value1.x - value2.x;
	float y = value1.y - value2.y;
	float z = value1.z - value2.z;

	return x * x + y * y + z * z;
}

int cMath::Random(int r1, int r2)
{
	return (int)(rand() % (r2 - r1 + 1)) + r1;
}