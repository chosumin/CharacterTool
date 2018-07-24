#pragma once
#include "cBasicGizmo.h"

class cRotatingGizmo : public cBasicGizmo
{
public:
	cRotatingGizmo(weak_ptr<sGlobalVariable> global);
	~cRotatingGizmo();
public:
	// cBasicGizmo을(를) 통해 상속됨
	virtual void Update(const D3DXMATRIX & gizmoWorld, const D3DXMATRIX & gizmoLocal, const D3DXVECTOR3 & mousePos, const D3DXVECTOR3 & mouseDir) override;
	virtual void Update(const D3DXMATRIX & gizmoLocal, const D3DXVECTOR3 & mousePos, const D3DXVECTOR3 & mouseDir) override;
private:
	void SetFixedMatrix(bool isWorld, const D3DXMATRIX& local);
	D3DXVECTOR3 GetDelta(const D3DXMATRIX& colMatrix, const D3DXMATRIX* updateMatrix = nullptr);
private:
	D3DXMATRIX _fixedMatrix;
};