#pragma once
#include "./Interface/iCollidable.h"
#include "./Interface/iRenderable.h"

struct sTransform;
class cCollider : public iCollidable, public iRenderable
{
public:
	cCollider(weak_ptr<sTransform> boneTransform,
			  D3DXVECTOR3 center);
	virtual ~cCollider();

	void Update();

	// iRenderable을(를) 통해 상속됨
	virtual void Render() override;
private:
	void SetLocalTransform();
protected:
	unique_ptr<cWorldBuffer> _worldBuffer;

	D3DXMATRIX _world;
	D3DXVECTOR3 _scale;

	weak_ptr<sTransform> _boneTransform;
	shared_ptr<sTransform> _localTransform;
	
	//todo : localTransform의 포지션으로 변경
	D3DXVECTOR3 _center;

	shared_ptr<cShader> _shader;
	unique_ptr<class cColliderBuffer> _cbuffer;

	//todo : 피격, 공격 구분하기
};