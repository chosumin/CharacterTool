#pragma once
#include "cCollider.h"

class cRayCollider : public cCollider
{
public:
	cRayCollider(weak_ptr<sTransform> parent,
				 D3DXVECTOR3 origin, D3DXVECTOR3 direction);
	~cRayCollider();

	D3DXVECTOR3 GetTransformedOrigin();
	D3DXVECTOR3 GetTransformedDirection();
private:
	/********************************
		cCollider을(를) 통해 상속됨
	*********************************/

	virtual ContainmentType ContainsRay(weak_ptr<cCollider> other) override;

	virtual ContainmentType ContainsPlane(weak_ptr<cCollider> other) override;

	virtual ContainmentType ContainsDot(weak_ptr<cCollider> other) override;

	virtual ContainmentType ContainsSphere(weak_ptr<cCollider> other) override;

	virtual ContainmentType ContainsBox(weak_ptr<cCollider> other) override;

	virtual bool IntersectsWithRay(weak_ptr<cCollider> other) override;

	virtual bool IntersectsWithQuad(weak_ptr<cCollider> other) override;

	virtual bool IntersectsWithDot(weak_ptr<cCollider> other) override;

	virtual bool IntersectsWithSphere(weak_ptr<cCollider> other) override;

	virtual bool IntersectsWithBox(weak_ptr<cCollider> other) override;

	virtual bool IntersectsWithCylinder(weak_ptr<cCollider> other) override;
private:
	D3DXVECTOR3 _origin;
	D3DXVECTOR3 _direction;
};