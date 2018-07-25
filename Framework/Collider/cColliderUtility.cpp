#include "stdafx.h"
#include "cColliderUtility.h"
#include "./Helper/cMath.h"

bool cColliderUtility::BoxAndRay(const D3DXVECTOR3 & boxMax, const D3DXVECTOR3 & boxMin, const D3DXVECTOR3 & position, const D3DXVECTOR3 & direction)
{
	float single = 0.0f;
	float single1 = cMath::FloatMaxValue;

	sRay ray;
	ray.position = position; ray.direction = direction;

	sBox box;
	box.max = boxMax; box.min = boxMin;

	if (!CheckBasis(single, single1, 0, ray, box))
		return false;
	if (!CheckBasis(single, single1, 1, ray, box))
		return false;
	if (!CheckBasis(single, single1, 2, ray, box))
		return false;

	return true;
}

bool cColliderUtility::RayAndQuad(const vector<D3DXVECTOR3>& fourPoints, const D3DXVECTOR3 & position, const D3DXVECTOR3 & direction)
{
	float u, v, distance;
	if (D3DXIntersectTri(&fourPoints[0], &fourPoints[1], &fourPoints[2], &position, &direction, &u, &v, &distance) == TRUE)
		return true;

	if (D3DXIntersectTri(&fourPoints[3], &fourPoints[1], &fourPoints[2], &position, &direction, &u, &v, &distance) == TRUE)
		return true;

	return false;
}

bool cColliderUtility::CheckBasis(float& single, float& single1, int i, const sRay& ray, const sBox& box)
{
	if (abs(ray.direction[i]) >= 1E-06f)
	{
		float x = 1.0f / ray.direction[i];
		float x1 = (box.min[i] - ray.position[i]) * x;
		float x2 = (box.max[i] - ray.position[i]) * x;
		if (x1 > x2)
		{
			float single2 = x1;
			x1 = x2;
			x2 = single2;
		}
		single = max(x1, single);
		single1 = min(x2, single1);
		if (single > single1)
			return false;
	}
	else if (ray.position[i] < box.min[i] || ray.position[i] > box.max[i])
		return false;

	return true;
}
