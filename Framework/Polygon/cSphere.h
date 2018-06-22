#pragma once
#include "./Interface/iTransformable.h"

class cSphere
{
public:
	cSphere(D3DXCOLOR color, D3DXVECTOR3 center, float radius);
	~cSphere();

	void Render();
private:
	void CreateVertex(D3DXVECTOR3 center, float radius);
private:
	D3DXCOLOR _color;

	vector<VertexC> _vertexData;

	ID3D11Buffer *_vb;
	ID3D11Buffer *_ib;
	
	D3DXVECTOR3 _center;
	float _radius;
};