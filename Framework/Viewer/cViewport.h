#pragma once

class cViewport
{
public:
	cViewport(float width, float height, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 1);
	~cViewport();

	void RSSetcViewport();
	void Set(float width, float height, float x = 0, float y = 0, float minDepth = 0, float maxDepth = 1);

	float GetWidth() { return m_fWidth; }
	float GetHeight() { return m_fHeight; }

	void GetMatrix(D3DXMATRIX *matrix) const;
private:
	float m_fX;
	float m_fY;
	float m_fWidth;
	float m_fHeight;
	float m_fMinDepth;
	float m_fMaxDepth;

	D3D11_VIEWPORT m_stViewport;
	CD3D11_VIEWPORT m_viewport;
};