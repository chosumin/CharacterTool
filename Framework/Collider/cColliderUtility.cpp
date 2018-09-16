#include "stdafx.h"
#include "cColliderUtility.h"
#include "./Helper/cMath.h"
#include <limits.h>

eContainmentType cColliderUtility::BoxAndRay(const D3DXVECTOR3 & boxMax, const D3DXVECTOR3 & boxMin, const D3DXVECTOR3 & position, const D3DXVECTOR3 & direction)
{
	float single = 0.0f;
	float single1 = FLT_MAX;

	sRay ray;
	ray.position = position; ray.direction = direction;

	sBox box;
	box.max = boxMax; box.min = boxMin;

	if (!CheckBasis(single, single1, 0, ray, box))
		return eContainmentType::Disjoint;
	if (!CheckBasis(single, single1, 1, ray, box))
		return eContainmentType::Disjoint;
	if (!CheckBasis(single, single1, 2, ray, box))
		return eContainmentType::Disjoint;

	return eContainmentType::Intersects;
}

eContainmentType cColliderUtility::RayAndQuad(const vector<D3DXVECTOR3>& fourPoints, const D3DXVECTOR3 & position, const D3DXVECTOR3 & direction)
{
	float u, v, distance;
	if (D3DXIntersectTri(&fourPoints[0], &fourPoints[1], &fourPoints[2], &position, &direction, &u, &v, &distance) == TRUE)
		return eContainmentType::Intersects;

	if (D3DXIntersectTri(&fourPoints[3], &fourPoints[1], &fourPoints[2], &position, &direction, &u, &v, &distance) == TRUE)
		return eContainmentType::Intersects;

	return eContainmentType::Disjoint;
}

eContainmentType cColliderUtility::CylinderAndSphere(const sLine & cylLine, float cylRadius, const D3DXVECTOR3 & sphCenter, float sphRadius)
{
	float distance = cMath::DistancePointFromLine(cylLine.start,
												  cylLine.end,
												  sphCenter);

	float sumRadius = cylRadius + sphRadius;

	if (distance > sumRadius) 
		return eContainmentType::Disjoint;
	else
	{
		if (distance + sphRadius <= cylRadius)
			return eContainmentType::Contains;

		return eContainmentType::Intersects;
	}

	return eContainmentType::Disjoint;
}

eContainmentType cColliderUtility::RayAndSphere(const D3DXVECTOR3 & rayPos, const D3DXVECTOR3 & rayDir, const D3DXVECTOR3 & sphCenter, float sphRadius)
{
	D3DXMATRIX invMat = cMath::MATRIX_IDENTITY;
	invMat._41 = -sphCenter.x;
	invMat._42 = -sphCenter.y;
	invMat._43 = -sphCenter.z;

	D3DXVECTOR3 origin, direction;
	D3DXVec3TransformCoord(&origin, &rayPos, &invMat);
	D3DXVec3TransformNormal(&direction, &rayDir, &invMat);

	float vv = D3DXVec3Dot(&direction, &direction);
	float qv = D3DXVec3Dot(&origin, &direction);
	float qq = D3DXVec3Dot(&origin, &origin);
	float rr = sphRadius * sphRadius;

	return qv * qv - vv * (qq - rr) < 0 ?
		eContainmentType::Disjoint : eContainmentType::Intersects;
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
