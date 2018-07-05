#pragma once
#include "./Message/cBaseGameEntity.h"

class cBasicMap : public cBaseGameEntity
{
public:
	using VertexType = VertexCTN;

public:
	cBasicMap(wstring file = L"", float heightRatio = 10.0f);
	virtual ~cBasicMap();

	virtual void Render();
public:
	float GetHeight(D3DXVECTOR3 position);
	void SetHeightMapFile(wstring file);
	void UpdateVertexData(D3D11_BOX* box = NULL);
public:
	// cBaseGameEntity을(를) 통해 상속됨
	virtual void HandleMessage(const sTelegram & msg) override;
private:
	// cBaseGameEntity을(를) 통해 상속됨
	virtual void FunctionInitialize() override;

	void Clear();

	void FillvertexData(float heightRatio);
	void FillNormalcBasicMap();

	void CreateBuffer();
protected:
	UINT			width;
	UINT			height;
	VertexType*		vertexData;
	UINT			vertexCount;
private:
	shared_ptr<class cTexture> heightMap;
	UINT			indexCount;
	ID3D11Buffer*	vertexBuffer;
	ID3D11Buffer*	indexBuffer;
	UINT*			indexData;
	float			heightRatio;
};