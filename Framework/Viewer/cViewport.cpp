#include "stdafx.h"
#include "cViewport.h"

cViewport::cViewport(float width, float height, float x, float y, float minDepth, float maxDepth)
{
	Set(width, height, x, y, minDepth, maxDepth);
}

cViewport::~cViewport()
{
	
}

void cViewport::Set(float width, float height, float x, float y, float minDepth, float maxDepth)
{
	m_stViewport.TopLeftX = m_fX = x;
	m_stViewport.TopLeftY = m_fY = y;
	m_stViewport.Width = m_fWidth = width;
	m_stViewport.Height = m_fHeight = height;
	m_stViewport.MinDepth = m_fMinDepth = minDepth;
	m_stViewport.MaxDepth = m_fMaxDepth = maxDepth;

	m_viewport.TopLeftX = m_fX = x;
	m_viewport.TopLeftY = m_fY = y;
	m_viewport.Width = m_fWidth = width;
	m_viewport.Height = m_fHeight = height;
	m_viewport.MinDepth = m_fMinDepth = minDepth;
	m_viewport.MaxDepth = m_fMaxDepth = maxDepth;

	RSSetcViewport();
}

void cViewport::GetMatrix(D3DXMATRIX *matrix) const
{
	D3DXMatrixIdentity(matrix);

	matrix->_11 = m_fWidth / 2;
	matrix->_22 = -m_fHeight / 2;
	matrix->_33 = m_fMaxDepth - m_fMinDepth;

	matrix->_41 = m_fX + matrix->_11;
	matrix->_42 = m_fY + matrix->_22;
	matrix->_43 = m_fMinDepth;
}

void cViewport::RSSetcViewport()
{
	D3D::GetDC()->RSSetViewports(1, &m_stViewport);
}
