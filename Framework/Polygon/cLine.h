#pragma once

class cLine
{
public:
	cLine(D3DXCOLOR color, D3DXVECTOR3 axis, float length);
	~cLine();

	virtual void Render();
private:
	void CreateVertex(D3DXVECTOR3 axis, float length);
	void RotateAxis(D3DXVECTOR3 axis);
private:
	D3DXCOLOR mColor;

	vector<VertexC> mVertexData;

	float mLength;

	ID3D11Buffer *mVB;
};