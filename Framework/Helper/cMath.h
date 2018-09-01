#pragma once
class cMath
{
public:
	static const float PI;
	static const D3DXMATRIX MATRIX_IDENTITY;
	static const int IntMinValue;
	static const int IntMaxValue;
	static const float FloatMinValue;
	static const float FloatMaxValue;

	static float ToRadian(float degree);
	static float ToDegree(float radian);

	static D3DXQUATERNION ToQuaternion(const D3DXVECTOR3& angles);

	static D3DXVECTOR3 ToEulerAngles(const D3DXQUATERNION& q);

	static int Random(int r1, int r2);
	static float Random(float r1, float r2);

	//���� min~max ���̷� ����
	static float Clamp(float value, float min, float max);

	//���� min~max ���̷� ����
	static D3DXVECTOR3 Clamp(const D3DXVECTOR3& value, const D3DXVECTOR3& min, const D3DXVECTOR3& max);

	//x*x, y*y, z*z
	static D3DXVECTOR3 Mul(D3DXVECTOR3& a, D3DXVECTOR3& b);

	//�� ������ �Ÿ��� ���� ��ȯ
	static float DistanceSquared(const D3DXVECTOR3& value1, const D3DXVECTOR3& value2);

	static bool IsVec3Equal(const D3DXVECTOR3& value, const D3DXVECTOR3& value2);
};