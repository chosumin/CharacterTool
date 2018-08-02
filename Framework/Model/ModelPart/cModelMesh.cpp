#include "stdafx.h"
#include "cModelMesh.h"
#include "cModelMeshPart.h"
#include "./Transform/sTransform.h"
#include "./Model/ModelPart/cModelBone.h"

cModelMesh::cModelMesh()
{
}

cModelMesh::~cModelMesh()
{
}

unique_ptr<cModelMesh> cModelMesh::Clone() const
{
	auto mesh = make_unique<cModelMesh>();
	mesh->_name = _name;
	mesh->_parentBoneIndex = _parentBoneIndex;
	for (auto&& part : _meshParts)
	{
		auto temp = part->Clone();
		mesh->_meshParts.emplace_back(move(temp));
	}

	return move(mesh);
}

weak_ptr<sTransform> cModelMesh::GetParentTransform() const
{
	return _parentBone.lock()->GetAbsoluteTransform();
}

void cModelMesh::Binding()
{
	for (auto&& part : _meshParts)
		part->Binding();
}

void cModelMesh::Render()
{
	for (auto&& part : _meshParts)
		part->Render();
}
