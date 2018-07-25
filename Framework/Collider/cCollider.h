#pragma once
#include "./Interface/iRenderable.h"
#include "./Interface/iCollidable.h"
#include "cColliderUtility.h"

enum class eColliderType
{
	NONE, ATTACK, DAMAGE
};

enum class eColliderShape : int
{
	BOX, CYLINDER, SPHERE, LINE, QUAD, RAY
};

struct sTransform;
class cCollider :
	public iRenderable,
	public iCollidable,
	public enable_shared_from_this<cCollider>
{
private:
	friend class cColliderFactory;
public:
	virtual ~cCollider();

	virtual void Update();
	virtual void Render() override;

	void ResetState();

	weak_ptr<sTransform> GetLocalTransform() const;
	void SetLocalTransform(const D3DXMATRIX& matrix);

	D3DXMATRIX& GetWorld() const;
	void SetWorld(const D3DXMATRIX& world);
	weak_ptr<sTransform> GetWorldTransform() const;

	eColliderType GetType() const { return _type; }
	eColliderShape GetShape() const { return _shape; }
protected:
	cCollider(weak_ptr<sTransform> parentTransform);
protected:
	shared_ptr<sTransform> _worldTransform;
	weak_ptr<sTransform> _parentTransform;
	shared_ptr<sTransform> _localTransform;

	unique_ptr<class cColliderBuffer> _cbuffer;
private:
	eColliderShape _shape;
	eColliderType _type;

	shared_ptr<cShader> _shader;
};