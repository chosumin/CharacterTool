#include "stdafx.h"
#include "cCamera.h"
#include "cPerspective.h"
#include "cViewport.h"

void cCamera::UpdateRotation()
{
	D3DXMATRIX x, y;
	D3DXMatrixRotationX(&x, m_vRotation.x);
	D3DXMatrixRotationY(&y, m_vRotation.y);

	m_matRotation = x * y;

	D3DXVec3TransformCoord(&m_vForward, &D3DXVECTOR3(0, 0, 1), &m_matRotation);
	D3DXVec3TransformCoord(&m_vRight, &D3DXVECTOR3(1, 0, 0), &m_matRotation);
	D3DXVec3TransformCoord(&m_vUp, &D3DXVECTOR3(0, 1, 0), &m_matRotation);
}

void cCamera::UpdateView()
{
	D3DXMatrixLookAtLH(&m_matView, &m_vPosition, &(m_vPosition + m_vForward), &m_vUp);
}

void cCamera::GetPosition(OUT D3DXVECTOR3 * pPosition)
{
	*pPosition = m_vPosition;
}

void cCamera::SetPosition(float x, float y, float z)
{
	m_vPosition = D3DXVECTOR3{ x,y,z };
	UpdateView();
}
//
//inline void cCamera::GetRotation(OUT D3DXVECTOR2 * pRotation)
//{
//	*pRotation = m_vRotation;
//}
//
//inline void cCamera::SetRotation(float x, float y)
//{
//	m_vRotation = D3DXVECTOR2{ x,y };
//	UpdateRotation();
//}
//
//inline void cCamera::GetRotationDegree(OUT D3DXVECTOR2 * pAngle)
//{
//	*pAngle = m_vRotation * 180.0f / (float)D3DX_PI;
//}
//
void cCamera::SetRotationDegree(float x, float y)
{
	m_vRotation = D3DXVECTOR2{ x,y } *(float)D3DX_PI / 180.0f;
}
//
void cCamera::GetMatrix(OUT D3DXMATRIX * pView)
{
	*pView = m_matView;
}

D3DXVECTOR3 cCamera::GetDirection(cViewport * pViewport
								  , cPerspective * pPerspective)
{
	D3DXVECTOR2 vScreenSize;
	vScreenSize.x = pViewport->GetWidth();
	vScreenSize.y = pViewport->GetHeight();

	auto vMouse = cMouse::Get()->GetPosition();
	D3DXVECTOR2 vPoint;
	D3DXMATRIX matProj;
	D3DXVECTOR3 vDirection;

	//Viewport
	{
		vPoint.x = ((2.0f * vMouse.x) / vScreenSize.x) - 1.0f;
		vPoint.y = (((2.0f * vMouse.y) / vScreenSize.y) - 1.0f) * -1.0f;
	}

	//Projection
	{
		pPerspective->GetMatrix(&matProj);

		vPoint.x = vPoint.x / matProj._11;
		vPoint.y = vPoint.y / matProj._22;
	}

	//View
	{
		D3DXMATRIX invView;
		D3DXMatrixInverse(&invView, NULL, &m_matView);

		vDirection.x = (vPoint.x * invView._11) + (vPoint.y * invView._21) + invView._31;
		vDirection.y = (vPoint.x * invView._12) + (vPoint.y * invView._22) + invView._32;
		vDirection.z = (vPoint.x * invView._13) + (vPoint.y * invView._23) + invView._33;

		D3DXVec3Normalize(&vDirection, &vDirection);
	}

	return vDirection;
}

void cCamera::Move(float x, float y, float z, float moveSpeed)
{
	m_vPosition += x * m_vForward * moveSpeed * cFrame::Delta();
	m_vPosition += y * m_vRight * moveSpeed * cFrame::Delta();
	m_vPosition += z * m_vUp * moveSpeed * cFrame::Delta();
}

void cCamera::Move(D3DXVECTOR3 vDirection, float moveSpeed)
{
	Move(vDirection.x, vDirection.y, vDirection.z, moveSpeed);
}

void cCamera::Rotate(float x, float y, float rotationSpeed)
{
	m_vRotation.x += y * rotationSpeed * cFrame::Delta();
	m_vRotation.y += x * rotationSpeed * cFrame::Delta();

	UpdateRotation();
}

void cCamera::Rotate(D3DXVECTOR3 vDirection, float rotationSpeed)
{
	Rotate(vDirection.x, vDirection.y, rotationSpeed);
}

cCamera::cCamera()
	:m_vPosition(0, 0, 0)
	, m_vRotation(0, 0)
	, m_vForward(0, 0, 1)
	, m_vRight(1, 0, 0)
	, m_vUp(0, 1, 0)
{
	D3DXMatrixIdentity(&m_matRotation);
	D3DXMatrixIdentity(&m_matView);

	UpdateRotation();
	UpdateView();
}

cCamera::~cCamera()
{
}
