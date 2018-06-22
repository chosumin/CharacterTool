#pragma once
class cMath
{
public:
	static const float PI;

	static const int IntMinValue;
	static const int IntMaxValue;
	static const float FloatMinValue;
	static const float FloatMaxValue;

	static float ToRadian(float degree);
	static float ToDegree(float radian);

	static int Random(int r1, int r2);
	static float Random(float r1, float r2);

	static float Clamp(float value, float min, float max);

	static D3DXVECTOR3 Mul(D3DXVECTOR3& a, D3DXVECTOR3& b);
};