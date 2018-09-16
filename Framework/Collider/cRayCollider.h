#pragma once
#include "cCollider.h"

class cRayCollider : public cCollider
{
public:
	cRayCollider(weak_ptr<sTransform> parent,
				 D3DXVECTOR3 origin, D3DXVECTOR3 direction);
	~cRayCollider();
public:
	/********************************
		cCollider을(를) 통해 상속됨
	*********************************/
	virtual eContainmentType Contains(const weak_ptr<iCollidable>& other) override;

	virtual eContainmentType ContainsRay(const D3DXVECTOR3 & position, const D3DXVECTOR3 & direction) override;
	virtual eContainmentType ContainsQuad(const vector<D3DXVECTOR3>& fourPoints) override;
	virtual eContainmentType ContainsDot(const D3DXVECTOR3 & point) override;
	virtual eContainmentType ContainsSphere(const D3DXVECTOR3 & center, float radius) override;
	virtual eContainmentType ContainsBox(const D3DXVECTOR3 & max, const D3DXVECTOR3 & min) override;
	virtual eContainmentType ContainsCylinder(const sLine & line, float radius) override;
	virtual ePlaneIntersectionType ContainsPlane(const D3DXVECTOR3 & normal, float d) override;
private:
	D3DXVECTOR3 GetTransformedOrigin();
	D3DXVECTOR3 GetTransformedDirection();
private:
	D3DXVECTOR3 _origin;
	D3DXVECTOR3 _direction;
};