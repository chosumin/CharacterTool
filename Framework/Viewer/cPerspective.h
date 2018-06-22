#pragma once

class cPerspective
{
public:
	cPerspective(float width, float height, float fov = (float)D3DX_PI * 0.25f, float zn = 0.1f, float zf = 1000.0f);
	~cPerspective();

	void GetMatrix(D3DXMATRIX* mat);
	void Set(float width, float height, float fov = (float)D3DX_PI * 0.25f, float zn = 0.1f, float zf = 1000.0f);
private:
	float m_fWidth;
	float m_fHeight;
	float m_fFov;
	float m_fAspect;
	float m_fZn;
	float m_fZf;

	D3DXMATRIX m_matProjection;
};