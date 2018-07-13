#pragma once
#include "./Interface/iRenderable.h"
#include "./Interface/iCollidable.h"

struct sTransform;
class cCollider :
	public iRenderable,
	public iCollidable,
	public enable_shared_from_this<cCollider>
{
public:
	cCollider(weak_ptr<sTransform> boneTransform);
	virtual ~cCollider();

	void Update();
	virtual void Render() override;

	weak_ptr<sTransform> GetLocalTransform() const;

	D3DXMATRIX GetWorld() const;
	void SetWorld(const D3DXMATRIX& world);
protected:
	shared_ptr<sTransform> _localTransform;

	unique_ptr<cWorldBuffer> _worldBuffer;

	D3DXMATRIX _world;
	D3DXVECTOR3 _scale;

	weak_ptr<sTransform> _boneTransform;

	shared_ptr<cShader> _shader;
	unique_ptr<class cColliderBuffer> _cbuffer;
};