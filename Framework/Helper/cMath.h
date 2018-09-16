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

	//@brief : 값을 min~max 사이로 조정
	static float Clamp(float value, float min, float max);

	//@brief : 값을 min~max 사이로 조정
	static D3DXVECTOR3 Clamp(const D3DXVECTOR3& value, const D3DXVECTOR3& min, const D3DXVECTOR3& max);

	//@brief : x*x, y*y, z*z
	static D3DXVECTOR3 Mul(D3DXVECTOR3& a, D3DXVECTOR3& b);

	//@brief : 두 벡터의 거리의 제곱 반환
	static float DistanceSquared(const D3DXVECTOR3& value1, const D3DXVECTOR3& value2);

	static bool IsVec3Equal(const D3DXVECTOR3& value, const D3DXVECTOR3& value2);

	//@brief : min, max 사이의 보간값 반환
	static float Lerp(float minVal, float maxVal, float i);

	//@brief : 두 벡터 사이의 각도 반환
	//@brief : twoPi = true면 360도 각 반환, false면 180도 각 반환
	static float AngleBetweenTwoVectors(const D3DXVECTOR3& vec1,
										const D3DXVECTOR3& vec2);

	//@brief : 두 점으로 생성된 선분과 한 점 사이간 거리
	//@param 1, 2 : 선분의 양 끝점
	//@param 3 : 비교할 점
	static float DistancePointFromLine(const D3DXVECTOR3& linePoint1,
									   const D3DXVECTOR3& linePoint2,
									   const D3DXVECTOR3& dotPoint);
};