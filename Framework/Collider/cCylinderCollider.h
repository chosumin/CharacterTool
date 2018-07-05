#pragma once
#include "cCollider.h"

struct sTransform;
class cCylinderCollider : public cCollider
{
public:
	cCylinderCollider(weak_ptr<sTransform> boneTransform,
					  D3DXVECTOR3 center,
					  float height, float radius);
	~cCylinderCollider();

	void Update();
	void Render();
public:
	/**********************************
		iCollidable을(를) 통해 상속됨
	***********************************/
	virtual ContainmentType ContainsRay(D3DXVECTOR3 position, D3DXVECTOR3 direction) override;
	virtual ContainmentType ContainsPlane(D3DXVECTOR3 normal, float d) override;
	virtual ContainmentType ContainsDot(D3DXVECTOR3 point) override;
	virtual ContainmentType ContainsSphere(D3DXVECTOR3 center, float radius) override;
	virtual ContainmentType ContainsBox(D3DXVECTOR3 max, D3DXVECTOR3 min) override;
	virtual bool IntersectsWithRay(D3DXVECTOR3 position, D3DXVECTOR3 direction) override;
	virtual PlaneIntersectionType IntersectsWithPlane(D3DXVECTOR3 normal, float d) override;
	virtual bool IntersectsWithDot(D3DXVECTOR3 point) override;
	virtual bool IntersectsWithSphere(D3DXVECTOR3 center, float radius) override;
	virtual bool IntersectsWithBox(D3DXVECTOR3 min, D3DXVECTOR3 max) override;
	virtual bool IntersectsWithCylinder(sLine line, float radius) override;
private:
	void CreateVertices();
	void CreateIndices();
private:
	sLine GetTransformedLine() const;
	float ClosestPtSegmentSegment(sLine line2);
private:
	float _height;
	float _radius;

	struct sMesh<VertexC> _mesh;
};