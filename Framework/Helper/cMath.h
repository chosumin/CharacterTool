#pragma once
class cMath
{
public:
	static const float PI;
	static const D3DXMATRIX MATRIX_IDENTITY;

	static float ToRadian(float degree);
	static float ToDegree(float radian);

	static D3DXQUATERNION ToQuaternion(const D3DXVECTOR3& angles);

	static D3DXVECTOR3 ToEulerAngles(const D3DXQUATERNION& q);

	static int Random(int r1, int r2);
	static float Random(float r1, float r2);

	//@brief : ���� min~max ���̷� ����
	static float Clamp(float value, float min, float max);

	//@brief : ���� min~max ���̷� ����
	static D3DXVECTOR3 Clamp(const D3DXVECTOR3& value, const D3DXVECTOR3& min, const D3DXVECTOR3& max);

	//@brief : x*x, y*y, z*z
	static D3DXVECTOR3 Mul(D3DXVECTOR3& a, D3DXVECTOR3& b);

	//@brief : �� ������ �Ÿ��� ���� ��ȯ
	static float DistanceSquared(const D3DXVECTOR3& value1, const D3DXVECTOR3& value2);

	static bool IsVec3Equal(const D3DXVECTOR3& value, const D3DXVECTOR3& value2);

	//@brief : min, max ������ ������ ��ȯ
	static float Lerp(float minVal, float maxVal, float i);

	//@brief : �� ���� ������ ���� ��ȯ
	//@brief : twoPi = true�� 360�� �� ��ȯ, false�� 180�� �� ��ȯ
	static float AngleBetweenTwoVectors(const D3DXVECTOR3& vec1,
										const D3DXVECTOR3& vec2);

	//@brief : �� ������ ������ ���а� �� �� ���̰� �Ÿ�
	//@param 1, 2 : ������ �� ����
	//@param 3 : ���� ��
	static float DistancePointFromLine(const D3DXVECTOR3& linePoint1,
									   const D3DXVECTOR3& linePoint2,
									   const D3DXVECTOR3& dotPoint);
};