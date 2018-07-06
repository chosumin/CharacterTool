#pragma once
#include "./Collider/cCollider.h"

class cBoxCollider : public cCollider
{
public:
	cBoxCollider(weak_ptr<sTransform> parent,
				 D3DXVECTOR3 min, D3DXVECTOR3 max);
	virtual ~cBoxCollider();

	virtual void Render() override;

	void ResetState();
public:
	// cCollider을(를) 통해 상속됨
	virtual bool IntersectsWith(weak_ptr<cCollider> other) override;

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
	bool CheckBasis(float& single, float& single1, int i, D3DXVECTOR3 pos, D3DXVECTOR3 dir) const;
private:
	const int _cornerCount = 8;
	D3DXVECTOR3 _min;
	D3DXVECTOR3 _max;

	D3DXVECTOR3 _originMin;
	D3DXVECTOR3 _originMax;

	unique_ptr<class cBox> _box;
};