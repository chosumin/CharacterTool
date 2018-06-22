#pragma once
#include "./Interface/iRenderable.h"

class cWorldGrid : public iRenderable
{
public:
	cWorldGrid();
	~cWorldGrid();
public:
	// iRenderable을(를) 통해 상속됨
	virtual void Render() override;
private:
	void CreateVertices(int nNumHalfTile = 100, float fInterval = 1.0f);
private:
	using VertexType = VertexC;

	ID3D11Buffer *_vb;

	vector<VertexType> _vertices;

	unique_ptr<cWorldBuffer> _world;
	shared_ptr<cShader> _shader;
};