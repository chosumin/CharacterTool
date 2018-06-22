#pragma once
#include "cBasicGizmo.h"

class cRotatingGizmo : public cBasicGizmo
{
public:
	cRotatingGizmo(weak_ptr<sGlobalVariable> global);
	~cRotatingGizmo();
public:
	// cBasicGizmo을(를) 통해 상속됨
	virtual void Update(const D3DXMATRIX & gizmoMatrix, const D3DXVECTOR3 & mousePos, const D3DXVECTOR3 & mouseDir) override;
private:
	D3DXVECTOR3 GetDelta(const D3DXMATRIX& matrix);
private:
	D3DXMATRIX _fixedMatrix;
};