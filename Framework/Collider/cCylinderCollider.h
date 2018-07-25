#pragma once
#include "cCollider.h"

struct sTransform;
class cCylinderCollider : public cCollider
{
public:
	cCylinderCollider(weak_ptr<sTransform> parentTransform,
					  float height, float radius);
	~cCylinderCollider();

	void Update();
	void Render() override;
public:
	/********************************
		cCollider을(를) 통해 상속됨
	*********************************/
	virtual bool IntersectsWith(weak_ptr<iCollidable> other) override;
	virtual ContainmentType ContainsRay(D3DXVECTOR3 position, D3DXVECTOR3 direction) override;
	virtual ContainmentType ContainsPlane(D3DXVECTOR3 normal, float d) override;
	virtual ContainmentType ContainsDot(D3DXVECTOR3 point) override;
	virtual ContainmentType ContainsSphere(D3DXVECTOR3 center, float radius) override;
	virtual ContainmentType ContainsBox(D3DXVECTOR3 max, D3DXVECTOR3 min) override;
	virtual bool IntersectsWithRay(D3DXVECTOR3 position, D3DXVECTOR3 direction) override;
	virtual PlaneIntersectionType IntersectsWithPlane(D3DXVECTOR3 normal, float d) override;
	virtual bool IntersectsWithQuad(const vector<D3DXVECTOR3>& fourPoints) override;
	virtual bool IntersectsWithDot(D3DXVECTOR3 point) override;
	virtual bool IntersectsWithSphere(D3DXVECTOR3 center, float radius) override;
	virtual bool IntersectsWithBox(D3DXVECTOR3 min, D3DXVECTOR3 max) override;
	virtual bool IntersectsWithCylinder(sLine line, float radius) override;
private:
	sLine GetTransformedLine() const;
	float GetTransformedRadius() const;
	float ClosestPtSegmentSegment(sLine line2);
private:
	float _height;
	float _radius;

	unique_ptr<class cCapsule> _capsule;
};