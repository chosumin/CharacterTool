#pragma once
#include "./Interface/iRenderable.h"
#include "./Message/cBaseGameEntity.h"

class cWorldGrid :
	public iRenderable,
	public cBaseGameEntity
{
public:
	cWorldGrid(weak_ptr<sGlobalVariable> global);
	~cWorldGrid();

	void Init();
public:
	void Update();
	void PreRender();

	// iRenderable을(를) 통해 상속됨
	virtual void Render() override;

	// cBaseGameEntity을(를) 통해 상속됨
	virtual void HandleMessage(const sTelegram & msg) override;
	virtual void FunctionInitialize() override;
private:
	void CreatePlane(float width = 1000.0f, float height = 1000.0f);
private:
	using VertexType = VertexC;

	ID3D11Buffer *_vb;

	vector<VertexType> _vertices;
	UINT _vertexSize;

	unique_ptr<cWorldBuffer> _world;
	shared_ptr<cShader> _shader;

	ID3D11BlendState* _alphaBlend[2];

	unique_ptr<cViewProjectionBuffer> _shadowBuffer;
	unique_ptr<class cDirectionalLight> _directionalLight;
};