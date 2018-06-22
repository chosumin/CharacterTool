#include "stdafx.h"
#include "cPerspective.h"

cPerspective::cPerspective(float width, float height, float fov, float zn, float zf)
{
	Set(width, height, fov, zn, zf);
}

cPerspective::~cPerspective()
{
}

void cPerspective::GetMatrix(D3DXMATRIX * mat)
{
	memcpy(mat, &m_matProjection, sizeof(D3DXMATRIX));
}

void cPerspective::Set(float width, float height, float fov, float zn, float zf)
{
	m_fWidth = width;
	m_fHeight = height;
	m_fFov = fov;
	m_fAspect = width / height;
	m_fZn = zn;
	m_fZf = zf;

	D3DXMatrixPerspectiveFovLH(&m_matProjection, fov, m_fAspect, zn, zf);
}
