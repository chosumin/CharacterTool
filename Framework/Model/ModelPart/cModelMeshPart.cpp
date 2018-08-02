#include "stdafx.h"
#include "cModelMeshPart.h"
#include "./Graphic/cMaterial.h"

cModelMeshPart::cModelMeshPart()
	: _vb(nullptr)
	, _ib(nullptr)
{
}

cModelMeshPart::~cModelMeshPart()
{
	SAFE_RELEASE(_vb);
	SAFE_RELEASE(_ib);
}

void cModelMeshPart::Binding()
{
	HRESULT hr;
	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;

	//Vertex
	{
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(ModelVertexType) * _vertices.size();
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = &_vertices[0];

		hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &_vb);
		assert(SUCCEEDED(hr));
	}

	//Index
	{
		ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.ByteWidth = sizeof(UINT) * _indices.size();
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		ZeroMemory(&data, sizeof(D3D11_SUBRESOURCE_DATA));
		data.pSysMem = &_indices[0];

		hr = D3D::GetDevice()->CreateBuffer(&desc, &data, &_ib);
		assert(SUCCEEDED(hr));
	}
}

void cModelMeshPart::Render()
{
	UINT stride = sizeof(ModelVertexType);
	UINT offset = 0;

	D3D::GetDC()->IASetVertexBuffers(0, 1, &_vb, &stride, &offset);
	D3D::GetDC()->IASetIndexBuffer(_ib, DXGI_FORMAT_R32_UINT, 0);
	D3D::GetDC()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if(!_material.expired())
		_material.lock()->Render();

	D3D::GetDC()->DrawIndexed(_indices.size(), 0, 0);
}

unique_ptr<cModelMeshPart> cModelMeshPart::Clone() const
{
	auto part = make_unique<cModelMeshPart>();
	part->_materialName = _materialName;
	part->_vertices.assign(_vertices.begin(), _vertices.end());
	part->_indices.assign(_indices.begin(), _indices.end());

	return move(part);
}
