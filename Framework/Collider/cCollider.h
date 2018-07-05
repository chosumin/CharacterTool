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

	// iRenderable��(��) ���� ��ӵ�
	virtual void Render() override;
private:
	void SetLocalTransform();
protected:
	unique_ptr<cWorldBuffer> _worldBuffer;

	D3DXMATRIX _world;
	D3DXVECTOR3 _scale;

	weak_ptr<sTransform> _boneTransform;
	shared_ptr<sTransform> _localTransform;
	
	//todo : localTransform�� ���������� ����
	D3DXVECTOR3 _center;

	shared_ptr<cShader> _shader;
	unique_ptr<class cColliderBuffer> _cbuffer;

	//todo : �ǰ�, ���� �����ϱ�
};