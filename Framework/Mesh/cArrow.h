#pragma once
#include "./Interface/iRenderable.h"

class cArrow : public iRenderable
{
public:
	cArrow(D3DXCOLOR color, D3DXVECTOR3 axis, float length);
	~cArrow();

	virtual void Render() override;
private:
	void CreateVertex(D3DXVECTOR3 axis, float length);
	void RotateAxis(D3DXVECTOR3 axis);
private:
	D3DXCOLOR mColor;

	vector<VertexC> mVertexData;

	float mLength;

	ID3D11Buffer *mVB;
	ID3D11Buffer *mIB;
	UINT mCount;
};