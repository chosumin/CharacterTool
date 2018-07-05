#include "stdafx.h"
#include "cModelMesh.h"
#include "cModelMeshPart.h"

#include "./Transform/sTransform.h"
#include "./Model/ModelPart/cModelBone.h"

cModelMesh::cModelMesh()
{
	_worldBuffer = make_unique<cWorldBuffer>();
}

cModelMesh::~cModelMesh()
{
}

unique_ptr<cModelMesh> cModelMesh::Clone() const
{
	auto mesh = make_unique<cModelMesh>();
	mesh->_name = _name;
	mesh->_parentBoneIndex = _parentBoneIndex;
	mesh->_worldBuffer = make_unique<cWorldBuffer>(*_worldBuffer);
	for (auto&& part : _meshParts)
	{
		auto temp = part->Clone();
		mesh->_meshParts.emplace_back(move(temp));
	}

	return move(mesh);
}

weak_ptr<sTransform> cModelMesh::GetParentTransform() const
{
	return _parentBone.lock()->GetTransform();
}

D3DXVECTOR3 cModelMesh::GetMeshPoint() const
{
	if (_meshParts.empty())
		return{ 0,0,0 };

	if (_meshParts[0]->_vertices.empty())
		return{ 0,0,0 };

	return _meshParts[0]->_vertices[0].position;
}

void cModelMesh::Binding()
{
	for (auto&& part : _meshParts)
		part->Binding();
}

void cModelMesh::Render()
{
	_worldBuffer->SetVSBuffer(1);
	for (auto&& part : _meshParts)
		part->Render();
}
