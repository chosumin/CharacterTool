#pragma once
#include "./Interface/iCollidable.h"

class cQuadCollider : public iCollidable
{
public:
	cQuadCollider(D3DXVECTOR3 min, D3DXVECTOR3 max, D3DXCOLOR color = { 0,0,0,1 });
	~cQuadCollider();

	void Render();

	void ResetState();
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
private:
	shared_ptr<cShader> _shader;
	unique_ptr<class cColliderBuffer> _cbuffer;

	unique_ptr<class cRectangle> _rect;
};