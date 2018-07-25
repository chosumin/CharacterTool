#pragma once

class cRectangle
{
public:
	cRectangle(D3DXVECTOR3 min, D3DXVECTOR3 max, D3DXCOLOR color = { 0,0,0,1 });
	~cRectangle();
public:
	void GetVertexVector(OUT vector<D3DXVECTOR3>& vec);
	bool IntersectWithRay(const D3DXVECTOR3& position, const D3DXVECTOR3& direction) const;

	//�ٿ�� �ڽ� ���� ������Ʈ
	void UpdateData(D3DXVECTOR3 start, D3DXVECTOR3 end);
	void UpdateVertexBuffer();
	void Render();
private:
	void CreateBuffer();
private:
	D3DXCOLOR		m_color;
	ID3D11Buffer*	m_pVB;
	ID3D11Buffer*	m_pIB;
	vector<VertexC>	m_vertexData;
	UINT*			m_arrIndexData;
};