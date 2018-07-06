#pragma once
#include "cCollider.h"

struct sTransform;
class cCylinderCollider : public cCollider
{
public:
	struct sLine { D3DXVECTOR3 start; D3DXVECTOR3 end; };
public:
	cCylinderCollider(weak_ptr<sTransform> boneTransform,
					  float height, float radius);
	~cCylinderCollider();

	void Update();
	void Render();

	sLine GetTransformedLine() const;
	float GetTransformedRadius() const;
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
	void CreateVertices();
	void CreateIndices();
private:
	float ClosestPtSegmentSegment(sLine line2);
private:
	float _height;
	float _radius;

	struct sMesh<VertexC> _mesh;
};