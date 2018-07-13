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
	virtual bool IntersectsWith(weak_ptr<iCollidable> other) override;

	virtual ContainmentType ContainsRay(D3DXVECTOR3 position, D3DXVECTOR3 direction) override;
	virtual ContainmentType ContainsPlane(D3DXVECTOR3 normal, float d) override;
	virtual ContainmentType ContainsDot(D3DXVECTOR3 point) override;
	virtual ContainmentType ContainsSphere(D3DXVECTOR3 center, float radius) override;
	virtual ContainmentType ContainsBox(D3DXVECTOR3 max, D3DXVECTOR3 min) override;
	virtual bool IntersectsWithRay(D3DXVECTOR3 position, D3DXVECTOR3 direction) override;
	virtual PlaneIntersectionType IntersectsWithPlane(D3DXVECTOR3 normal, float d) override;
	virtual bool IntersectsWithQuad(const cRectangle & rect) override;
	virtual bool IntersectsWithDot(D3DXVECTOR3 point) override;
	virtual bool IntersectsWithSphere(D3DXVECTOR3 center, float radius) override;
	virtual bool IntersectsWithBox(D3DXVECTOR3 min, D3DXVECTOR3 max) override;
	virtual bool IntersectsWithCylinder(sLine line, float radius) override;
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