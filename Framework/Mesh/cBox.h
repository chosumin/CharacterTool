#pragma once

class cBox
{
public:
	cBox(D3DXVECTOR3 min, D3DXVECTOR3 max, bool wireMode = false, D3DXCOLOR color = { 0,0,0,1 });
	~cBox();

public:
	//바운딩 박스 정점 업데이트
	void UpdateData(D3DXVECTOR3 start, D3DXVECTOR3 end);
	void UpdateVertexBuffer();
	void Render();
private:
	D3DXCOLOR _color;

	vector<unique_ptr<class cRectangle>> _rectangles;

	bool m_bWireMode;
	ID3D11RasterizerState* m_pRasterizer;
};