#pragma once
#include "cBasicGizmo.h"

class cMovingGizmo : public cBasicGizmo
{
public:
	cMovingGizmo(weak_ptr<sGlobalVariable> global);
	~cMovingGizmo();
public:
	// cBasicGizmo을(를) 통해 상속됨
	virtual void Update(const D3DXMATRIX & gizmoWorld, const D3DXMATRIX & gizmoLocal, const D3DXVECTOR3 & mousePos, const D3DXVECTOR3 & mouseDir) override;
	virtual void Update(const D3DXMATRIX & gizmoLocal, const D3DXVECTOR3 & mousePos, const D3DXVECTOR3 & mouseDir) override;
private:
	void Move(const D3DXMATRIX& colMatrix, const D3DXMATRIX* updateMatrix = nullptr);
};