#pragma once
#include "cBoxCollider.h"

class cBoxLineCollider : public cBoxCollider
{
public:
	cBoxLineCollider(float length, D3DXCOLOR color, D3DXVECTOR3 direction, D3DXVECTOR3 min, D3DXVECTOR3 max);
	~cBoxLineCollider();

	//·»´õ¸µ
	virtual void Render() override;
private:
	unique_ptr<class cLine> _line;
	unique_ptr<class cBox> _box;
};