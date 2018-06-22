#pragma once
#include "cBasicGizmo.h"

class cScalingGizmo : public cBasicGizmo
{
public:
	cScalingGizmo(weak_ptr<sGlobalVariable> global);
	~cScalingGizmo();
public:
	// cBasicGizmo��(��) ���� ��ӵ�
	virtual void Update(const D3DXMATRIX & gizmoMatrix, const D3DXVECTOR3 & mousePos, const D3DXVECTOR3 & mouseDir) override;
private:
	virtual void Scale(const D3DXMATRIX& matrix) ;
};