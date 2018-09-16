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

bool cMath::IsVec3Equal(const D3DXVECTOR3 & value, const D3DXVECTOR3 & value2)
{
	if (fabs(value.x - value2.x) > D3DX_16F_EPSILON ||
		fabs(value.y - value2.y) > D3DX_16F_EPSILON ||
		fabs(value.z - value2.z) > D3DX_16F_EPSILON)
		return false;

	return true;
}

float cMath::Lerp(float minVal, float maxVal, float i)
{
	return (1 - i) * minVal + i * maxVal;
}

float cMath::AngleBetweenTwoVectors(const D3DXVECTOR3 & vec1,
									const D3DXVECTOR3 & vec2)
{
	D3DXVECTOR3 dir1 = vec1;
	D3DXVec3Normalize(&dir1, &dir1);

	D3DXVECTOR3 dir2 = vec2;
	D3DXVec3Normalize(&dir2, &dir2);

	float dot = D3DXVec3Dot(&dir1, &dir2);

	//float dot = D3DXVec3Dot(&vec1, &vec2);

	dot = acosf(dot);
	return dot / RADIAN;
}

float cMath::DistancePointFromLine(const D3DXVECTOR3 & linePoint1, const D3DXVECTOR3 & linePoint2, const D3DXVECTOR3 & dotPoint)
{
	if (linePoint1 == dotPoint || linePoint2 == dotPoint)
		return 0.0f;

	D3DXVECTOR3 d0 = linePoint1 - dotPoint;
	D3DXVECTOR3 d1 = linePoint2 - dotPoint;
	D3DXVECTOR3 d2 = linePoint2 - linePoint1;

	float a = D3DXVec3Length(&d0);
	float b = D3DXVec3Length(&d1);
	float c = D3DXVec3Length(&d2);

	//선분 길이가 0일 때
	if (c == 0.0f) return a;

	D3DXVec3Normalize(&d0, &d0);
	D3DXVec3Normalize(&d1, &d1);
	D3DXVec3Normalize(&d2, &d2);

	float angle0 = acosf(D3DXVec3Dot(&d1, &d2));
	float angle1 = acosf(D3DXVec3Dot(&-d0, &d2)); //예각을 위해 -취함
	float angle2 = acosf(D3DXVec3Dot(&d0, &d1));

	//선분에서 수선을 내릴 수 없다면 선분 끝점과의 거리를 바로 구함
	if (angle0 > PI / 2.0f)
		return D3DXVec3Length(&(linePoint2 - dotPoint));

	if (angle1 > PI / 2.0f)
		return D3DXVec3Length(&(linePoint1 - dotPoint));

	//선분과 매우 길이가 멀 때
	if (angle2 == 0.0f)
		return a <= b ? a : b;

	//선분과 초근접
	if (angle2 >= PI - D3DX_16F_EPSILON)
		return 0.0f;

	return a* b * sinf(angle2) / c;

}

int cMath::Random(int r1, int r2)
{
	return (int)(rand() % (r2 - r1 + 1)) + r1;
}