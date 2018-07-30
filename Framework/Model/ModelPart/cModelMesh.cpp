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

const vector<shared_ptr<cCollider>>& cModelMesh::GetColliders() const
{
	return _colliders;
}

void cModelMesh::AddCollider(weak_ptr<cCollider> collider)
{
	_colliders.emplace_back(collider.lock());
}

void cModelMesh::DeleteCollider(weak_ptr<cCollider> collider)
{
	auto colPtr = collider.lock();
	for (auto iter = _colliders.begin(); iter != _colliders.end(); iter++)
	{
		if (*iter == colPtr)
		{
			_colliders.erase(iter);
			break;
		}
	}
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
