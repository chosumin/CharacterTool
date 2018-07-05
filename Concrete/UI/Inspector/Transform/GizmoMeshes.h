#pragma once
#include "./Interface/iRenderable.h"

class cRotateLine : public iRenderable
{
public:
	cRotateLine(float length, D3DXCOLOR color, D3DXVECTOR3 direction);
	~cRotateLine();

	// iRenderable을(를) 통해 상속됨
	virtual void Render() override;
private:
	void CreateVertices(D3DXCOLOR color);
	void CreateIndices();
	void RotateAxis(D3DXVECTOR3 axis, float length);
private:
	unique_ptr<class cLine> _line;

	vector<VertexC> _vertices;
	vector<UINT> _indices;

	ID3D11Buffer *_vb;
	ID3D11Buffer *_ib;
};

//////////////////////////////////////////////////////////////

class cBoxLine : public iRenderable
{
public:
	cBoxLine(float length, D3DXCOLOR color, D3DXVECTOR3 direction, D3DXVECTOR3 min, D3DXVECTOR3 max);
	~cBoxLine();

	// iRenderable을(를) 통해 상속됨
	virtual void Render() override;
private:
	unique_ptr<class cLine> _line;
	unique_ptr<class cBox> _box;
};