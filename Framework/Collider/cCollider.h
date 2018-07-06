#pragma once
#include "./Interface/iRenderable.h"

enum class ContainmentType
{
	Disjoint, Contains, Intersects
};

enum class eColliderShape
{
	Box, Cylinder, Sphere, Ray, Quad
};

struct sTransform;
class cCollider : public iRenderable,
	public enable_shared_from_this<cCollider>
{
public:
	cCollider(weak_ptr<sTransform> boneTransform, eColliderShape shape);
	virtual ~cCollider();

	void Update();

	bool IntersectsWith(weak_ptr<cCollider> other);
	virtual void Render() override;

	weak_ptr<sTransform> GetLocalTransform() const;
	void SetWorld(const D3DXMATRIX& world);
protected:
	virtual ContainmentType ContainsRay(weak_ptr<cCollider> other) = 0;
	virtual ContainmentType ContainsPlane(weak_ptr<cCollider> other) = 0;
	virtual ContainmentType ContainsDot(weak_ptr<cCollider> other) = 0;
	virtual ContainmentType ContainsSphere(weak_ptr<cCollider> other) = 0;
	virtual ContainmentType ContainsBox(weak_ptr<cCollider> other) = 0;

	virtual bool IntersectsWithRay(weak_ptr<cCollider> other) = 0;
	virtual bool IntersectsWithQuad(weak_ptr<cCollider> other) = 0;
	virtual bool IntersectsWithDot(weak_ptr<cCollider> other) = 0;
	virtual bool IntersectsWithSphere(weak_ptr<cCollider> other) = 0;
	virtual bool IntersectsWithBox(weak_ptr<cCollider> other) = 0;
	virtual bool IntersectsWithCylinder(weak_ptr<cCollider> other) = 0;
protected:
	eColliderShape _shape;

	unique_ptr<cWorldBuffer> _worldBuffer;

	D3DXMATRIX _world;
	D3DXVECTOR3 _scale;

	weak_ptr<sTransform> _boneTransform;
	shared_ptr<sTransform> _localTransform;

	shared_ptr<cShader> _shader;
	unique_ptr<class cColliderBuffer> _cbuffer;
};