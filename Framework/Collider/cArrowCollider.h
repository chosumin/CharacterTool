#pragma once
#include "cBoxCollider.h"

class cArrowCollider : public cBoxCollider
{
public:
	cArrowCollider(float length, D3DXCOLOR color, D3DXVECTOR3 direction, D3DXVECTOR3 min, D3DXVECTOR3 max);
	~cArrowCollider();

	//·»´õ¸µ
	virtual void Render() override;
private:
	unique_ptr<class cArrow> _arrow;
};