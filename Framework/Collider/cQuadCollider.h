#pragma once
#include "./Collider/cCollider.h"

class cQuadCollider : public cCollider
{
public:
	cQuadCollider(weak_ptr<sTransform> parent,
				  D3DXVECTOR3 min, D3DXVECTOR3 max,
				  D3DXCOLOR color = { 0,0,0,1 });
	~cQuadCollider();

	void Render();

	void ResetState();
public:
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
	unique_ptr<class cRectangle> _rect;
};