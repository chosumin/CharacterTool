#pragma once
#include "./Message/cBaseGameEntity.h"

class cWorldMap : public cBaseGameEntity
{
private:
	using VertexType = VertexCTN;
public:
	cWorldMap();
	~cWorldMap();

	// cBaseGameEntity을(를) 통해 상속됨
	virtual void HandleMessage(const sTelegram & msg) override;

	void Render();
private:
	// cBaseGameEntity을(를) 통해 상속됨
	virtual void FunctionInitialize() override;
private:
	shared_ptr<class cTexture> _heightMap;
	UINT _width, _height;
	unique_ptr<sMesh<VertexType>> _mesh;
};