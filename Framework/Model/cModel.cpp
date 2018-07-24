#include "stdafx.h"
#include "cModel.h"
#include "./Model/cModelAnimPlayer.h"
#include "./ModelPart/cModelBone.h"
#include "./ModelPart/cModelMesh.h"
#include "./ModelPart/cModelMeshPart.h"
#include "./ModelPart/cModelAnimClip.h"
#include "./Graphic/cMaterial.h"
#include "./Graphic/ConstBuffer/cModelBoneBuffer.h"
#include "./Transform/sTransform.h"
#include "./Helper/cString.h"

cModel::cModel()
{
	_buffer = make_shared<cModelBoneBuffer>();
}

std::unique_ptr<cModel> cModel::Clone() const
{
	//트릭 구조체
	struct make_unique_enabler : public cModel {};
	auto model = make_unique<make_unique_enabler>();
	
	for (auto&& material : _materials)
		model->_materials.push_back(material->Clone());

	for (auto&& bone : _bones)
		model->_bones.push_back(bone->Clone());

	for (auto&& mesh : _meshes)
		model->_meshes.push_back(mesh->Clone());

	model->_root = model->GetRoot();

	return move(model);
}

cModel::~cModel()
{
	_materials.clear();
	_bones.clear();
	_meshes.clear();
	_clips.clear();

	_buffer = nullptr;
}

void cModel::Update(const D3DXMATRIX& root)
{
}

void cModel::Render()
{
	_buffer->SetVSBuffer(2);
	for (auto&& mesh : _meshes)
	{
		mesh->Render();
	}
}

vector<shared_ptr<cMaterial>>& cModel::GetMaterials()
{
	return _materials;
}

weak_ptr<cMaterial> cModel::GetMaterial(wstring name) const
{
	for (auto&& material : _materials)
	{
		if (material->GetName() == name)
			return material;
	}
	return weak_ptr<cMaterial>();
}

weak_ptr<cModelMesh> cModel::GetMesh(wstring name)
{
	for (auto&& mesh : _meshes)
	{
		if (mesh->_name == name)
			return mesh;
	}
	return weak_ptr<cModelMesh>();
}

weak_ptr<cModelBone> cModel::GetBone(wstring name)
{
	for (auto& bone : _bones)
	{
		if (bone->_name == name)
			return bone;
	}
	return weak_ptr<cModelBone>();
}

void cModel::CopyAbsoluteBoneTo(vector<D3DXMATRIX>& transforms)
{
	transforms.clear();
	transforms.assign(_bones.size(), D3DXMATRIX());

	for (size_t i = 0; i < _bones.size(); i++)
	{
		auto bone = _bones[i];

		//부모 노드의 좌표를 더한다
		if (bone->_parent.expired() == false)
		{
			int index = bone->_parent.lock()->_index;
			transforms[i] = bone->_transform->Matrix * transforms[index];
		}
		else
			transforms[i] = bone->_transform->Matrix;
	}
}