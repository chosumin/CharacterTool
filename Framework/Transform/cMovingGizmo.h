#pragma once
#include "cBasicGizmo.h"

class cMovingGizmo : public cBasicGizmo
{
public:
	cMovingGizmo(weak_ptr<sGlobalVariable> global);
	~cMovingGizmo();
public:
	// cBasicGizmo��(��) ���� ��ӵ�
	virtual void Update(const D3DXMATRIX & gizmoMatrix, const D3DXVECTOR3 & mousePos, const D3DXVECTOR3 & mouseDir) override;
private:
	void Move(const D3DXMATRIX& matrix);
};