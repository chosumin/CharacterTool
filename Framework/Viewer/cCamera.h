#pragma once
#include "./Common/cSingletone.h"

class cViewport;
class cPerspective;
class cCamera
{
public:
	cCamera();
	virtual ~cCamera();

	void GetPosition(OUT D3DXVECTOR3* pPosition);
	void SetPosition(float x, float y, float z);

	void GetRotation(OUT D3DXVECTOR2* pRotation);
	void SetRotation(float x, float y) = delete;

	void GetRotationDegree(OUT D3DXVECTOR2* pAngle) = delete;
	void SetRotationDegree(float x, float y);
	
	void GetMatrix(OUT D3DXMATRIX* pView);

	void GetCameraDirection(OUT D3DXVECTOR3& camDir);

	void GetMouseDirection(OUT D3DXVECTOR3& mouse);

	void GetRotationMatrix(OUT D3DXMATRIX& rotationMat);

	virtual void Move(float x, float y, float z, float moveSpeed);
	virtual void Move(D3DXVECTOR3 vDirection, float moveSpeed);
	virtual void Rotate(float x, float y, float rotationSpeed);
	virtual void Rotate(D3DXVECTOR3 vDirection, float rotationSpeed);
public:
	virtual void Update() = 0;
	virtual void PostRender() = 0;
protected:
	virtual void UpdateRotation();
	virtual void UpdateView();
protected:
	D3DXVECTOR3 m_vForward;
	D3DXVECTOR3 m_vRight;
	D3DXVECTOR3 m_vUp;

	D3DXVECTOR3 m_vPosition;
	D3DXVECTOR2 m_vRotation;
private:
	D3DXMATRIX m_matRotation;
	D3DXMATRIX m_matView;
};

