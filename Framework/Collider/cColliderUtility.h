#pragma once

class cColliderUtility
{
public:
	static bool BoxAndRay(D3DXVECTOR3 boxMax, D3DXVECTOR3 boxMin, D3DXVECTOR3 position, D3DXVECTOR3 direction);

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