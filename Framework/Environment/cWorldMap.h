#pragma once
#include "./Message/cBaseGameEntity.h"

class cWorldMap : public cBaseGameEntity
{
private:
	using VertexType = VertexCTN;
public:
	cWorldMap();
	~cWorldMap();

	// cBaseGameEntity��(��) ���� ��ӵ�
	virtual void HandleMessage(const sTelegram & msg) override;

	void Render();
private:
	// cBaseGameEntity��(��) ���� ��ӵ�
	virtual void FunctionInitialize() override;
private:
	shared_ptr<class cTexture> _heightMap;
	UINT _width, _height;
	unique_ptr<sMesh<VertexType>> _mesh;
};