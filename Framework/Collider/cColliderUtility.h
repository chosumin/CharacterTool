#pragma once
#include "./Interface/iCollidable.h"

class cColliderUtility
{
public:
	static eContainmentType BoxAndRay(const D3DXVECTOR3 & boxMax, const D3DXVECTOR3 & boxMin, const D3DXVECTOR3 & position, const D3DXVECTOR3 & direction);

	static eContainmentType RayAndQuad(const vector<D3DXVECTOR3>& fourPoints, const D3DXVECTOR3& position, const D3DXVECTOR3& direction);

	static eContainmentType CylinderAndSphere(const sLine& cylLine, float cylRadius, const D3DXVECTOR3& sphCenter, float sphRadius);

	static eContainmentType RayAndSphere(const D3DXVECTOR3& rayPos, const D3DXVECTOR3& rayDir, const D3DXVECTOR3& sphCenter, float sphRadius);
private:
	struct sBox
	{
		D3DXVECTOR3 max;
		D3DXVECTOR3 min;
	};

	struct sRay
	{
		D3DXVECTOR3 position;
		D3DXVECTOR3 direction;
	};

	static bool CheckBasis(float& single, float& single1, int i, const sRay& ray, const sBox& box);
};