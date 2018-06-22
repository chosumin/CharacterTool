#pragma once

//위치
struct Vertex
{
	Vertex()
		:position(0, 0, 0) {}

		D3DXVECTOR3 position;
};

///////////////////////////////////////////////

//위치 색깔
struct VertexC
{
	VertexC()
		:position(0,0,0)
		,color(0,0,0,1) {}

	D3DXVECTOR3 position;
	D3DXCOLOR color;
};

///////////////////////////////////////////////

//위치 법선
struct VertexCN
{
	VertexCN()
		:position(0,0,0)
		,color(0,0,0,1)
		,normal(0,0,0) {}

	D3DXVECTOR3 position;
	D3DXCOLOR color;
	D3DXVECTOR3 normal;
};

///////////////////////////////////////////////

//위치 텍스쳐UV
struct VertexT
{
	VertexT()
		: position(0, 0, 0)
		, uv(0, 0) {}

	D3DXVECTOR3	position;
	D3DXVECTOR2	uv;
};

///////////////////////////////////////////////

//위치 텍스쳐UV 법선
struct VertexTN
{
	VertexTN()
		: position(0, 0, 0)
		, uv(0, 0)
		, normal(0, 0, 0) {}

	D3DXVECTOR3 position;
	D3DXVECTOR2 uv;
	D3DXVECTOR3 normal;
};

///////////////////////////////////////////////

//위치 색깔 텍스쳐UV 법선
struct VertexCTN
{
	VertexCTN()
		: position(0, 0, 0)
		, color(0, 0, 0, 1)
		, uv(0, 0)
		, normal(0, 0, 0) {}

	D3DXVECTOR3 position;
	D3DXCOLOR color;
	D3DXVECTOR2 uv;
	D3DXVECTOR3 normal;
};

///////////////////////////////////////////////

//위치 텍스쳐UV 법선 블렌드인덱스 블렌드가중치
struct VertexTNB
{
	VertexTNB()
		: position(0, 0, 0)
		, uv(0, 0)
		, normal(0, 0, 0)
		, blendIndices(0, 0, 0, 0)
		, blendWeights(0, 0, 0, 0) {}

	D3DXVECTOR3 position;
	D3DXVECTOR2 uv;
	D3DXVECTOR3 normal;
	D3DXVECTOR4 blendIndices;
	D3DXVECTOR4 blendWeights;
};

///////////////////////////////////////////////

//위치 텍스쳐UV 법선 탄젠트
struct VertexTNT
{
	VertexTNT()
		: position(0, 0, 0)
		, uv(0, 0)
		, normal(0, 0, 0)
		, tangent(0, 0, 0)
	{}

	D3DXVECTOR3	position;
	D3DXVECTOR2	uv;
	D3DXVECTOR3	normal;
	D3DXVECTOR3 tangent;
};

///////////////////////////////////////////////

//위치 색깔1 색깔2
struct VertexTCC
{
	VertexTCC()
		: position(0, 0, 0)
		, uv(0, 0)
		, color(0, 0, 0, 1)
		, color2(0, 0, 0, 1)
	{

	}

	D3DXVECTOR3 position;
	D3DXVECTOR2 uv;
	D3DXCOLOR color;
	D3DXCOLOR color2;
};