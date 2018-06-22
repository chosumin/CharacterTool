#pragma once

//��ġ
struct Vertex
{
	Vertex()
		:position(0, 0, 0) {}

		D3DXVECTOR3 position;
};

///////////////////////////////////////////////

//��ġ ����
struct VertexC
{
	VertexC()
		:position(0,0,0)
		,color(0,0,0,1) {}

	D3DXVECTOR3 position;
	D3DXCOLOR color;
};

///////////////////////////////////////////////

//��ġ ����
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

//��ġ �ؽ���UV
struct VertexT
{
	VertexT()
		: position(0, 0, 0)
		, uv(0, 0) {}

	D3DXVECTOR3	position;
	D3DXVECTOR2	uv;
};

///////////////////////////////////////////////

//��ġ �ؽ���UV ����
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

//��ġ ���� �ؽ���UV ����
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

//��ġ �ؽ���UV ���� �����ε��� ���尡��ġ
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

//��ġ �ؽ���UV ���� ź��Ʈ
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

//��ġ ����1 ����2
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