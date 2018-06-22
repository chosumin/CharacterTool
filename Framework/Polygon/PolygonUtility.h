#pragma once
#include "stdafx.h"

template<typename T>
class cPolygon
{
public:
	static void CreateVertexBuffer(OUT ID3D11Buffer** vertexBuffer, const vector<T>& vecVertex)
	{
		UINT vertexCount = vecVertex.size();
		HRESULT hr;
		D3D11_BUFFER_DESC desc;
		D3D11_SUBRESOURCE_DATA data;

		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(T) * vertexCount;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = &vecVertex[0];

		hr = D3D::GetDevice()->CreateBuffer(&desc, &data, vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	static void CreateVertexBuffer(OUT ID3D11Buffer** vertexBuffer, const T* arrVertex, UINT vertexSize)
	{
		HRESULT hr;
		D3D11_BUFFER_DESC desc;
		D3D11_SUBRESOURCE_DATA data;

		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(T) * vertexSize;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = &arrVertex[0];

		hr = D3D::GetDevice()->CreateBuffer(&desc, &data, vertexBuffer);
		assert(SUCCEEDED(hr));
	}

	static void CreateIndexBuffer(OUT ID3D11Buffer** indexBuffer, const vector<UINT>& vecIndex)
	{
		UINT indexCount = vecIndex.size();

		HRESULT hr;
		D3D11_BUFFER_DESC desc;
		D3D11_SUBRESOURCE_DATA data;

		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * indexCount;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = &vecIndex[0];

		hr = D3D::GetDevice()->CreateBuffer(&desc, &data, indexBuffer);
		assert(SUCCEEDED(hr));
	}

	static void CreateIndexBuffer(OUT ID3D11Buffer** indexBuffer, const UINT* arrIndex, UINT indexSize)
	{
		HRESULT hr;
		D3D11_BUFFER_DESC desc;
		D3D11_SUBRESOURCE_DATA data;

		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * indexSize;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = &arrIndex[0];

		hr = D3D::GetDevice()->CreateBuffer(&desc, &data, indexBuffer);
		assert(SUCCEEDED(hr));
	}

	static void Render(ID3D11Buffer *VB, ID3D11Buffer *IB, UINT indicesSize, D3D_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		UINT stride = sizeof(T);
		UINT offset = 0;

		D3D::GetDC()->IASetVertexBuffers(0, 1, &VB, &stride, &offset);
		D3D::GetDC()->IASetIndexBuffer(IB, DXGI_FORMAT_R32_UINT, 0);

		D3D::GetDC()->IASetPrimitiveTopology(type);
		D3D::GetDC()->DrawIndexed(indicesSize, 0, 0);
	}

	static void Render(ID3D11Buffer *VB, UINT verticesSize, D3D_PRIMITIVE_TOPOLOGY type = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	{
		UINT stride = sizeof(T);
		UINT offset = 0;

		D3D::GetDC()->IASetVertexBuffers(0, 1, &VB, &stride, &offset);
		D3D::GetDC()->IASetPrimitiveTopology(type);
		D3D::GetDC()->Draw(verticesSize, 0);
	}
};