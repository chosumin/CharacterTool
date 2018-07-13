#pragma once
#include "./Interface/iRenderable.h"

class cCapsule : public iRenderable
{
public:
	cCapsule(float height, float radius, D3DXCOLOR color);
	~cCapsule();

	virtual void Render();
private:
	void CreateVertex(float height, float radius, D3DXCOLOR color);
	void CreateIndices();
private:
	D3DXCOLOR _color;
	struct sMesh<VertexC> _mesh;
};