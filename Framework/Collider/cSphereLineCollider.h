#pragma once
#include "cBoxCollider.h"

class cSphereLineCollider : public cBoxCollider
{
public:
	cSphereLineCollider(float length, D3DXCOLOR color, D3DXVECTOR3 direction, D3DXVECTOR3 min, D3DXVECTOR3 max);
	~cSphereLineCollider();

	//·»´õ¸µ
	virtual void Render() override;
private:
	void CreateVertices(D3DXCOLOR color);
	void CreateIndices();

	void RotateAxis(D3DXVECTOR3 axis, float length);
private:
	unique_ptr<class cLine> _line;
	unique_ptr<class cSphere> _sphere;

	vector<VertexC> _vertices;
	vector<UINT> _indices;

	ID3D11Buffer *_vb;
	ID3D11Buffer *_ib;
};