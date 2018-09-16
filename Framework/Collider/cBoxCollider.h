#pragma once
#include "./Collider/cCollider.h"

class cBoxCollider : public cCollider
{
public:
	cBoxCollider(weak_ptr<sTransform> parent,
				 D3DXVECTOR3 min, D3DXVECTOR3 max);
	virtual ~cBoxCollider();

	virtual void Render() override;
	virtual void RenderGizmo();
public:
	/********************************
		cCollider을(를) 통해 상속됨
	*********************************/
	virtual eContainmentType Contains(const weak_ptr<iCollidable>& other) override;

	virtual eContainmentType ContainsRay(const D3DXVECTOR3 & position, const D3DXVECTOR3 & direction) override;
	virtual eContainmentType ContainsQuad(const vector<D3DXVECTOR3>& fourPoints) override;
	virtual eContainmentType ContainsDot(const D3DXVECTOR3 & point) override;
	virtual eContainmentType ContainsSphere(const D3DXVECTOR3 & center, float radius) override;
	virtual eContainmentType ContainsBox(const D3DXVECTOR3 & max, const D3DXVECTOR3 & min) override;
	virtual eContainmentType ContainsCylinder(const sLine & line, float radius) override;
	virtual ePlaneIntersectionType ContainsPlane(const D3DXVECTOR3 & normal, float d) override;
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