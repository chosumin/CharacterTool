#include "stdafx.h"
#include "cModel.h"
#include "./ModelPart/cModelBone.h"
#include "./ModelPart/cModelMesh.h"
#include "./ModelPart/cModelMeshPart.h"
#include "./Graphic/cMaterial.h"
#include "./Graphic/ConstBuffer/cModelBoneBuffer.h"
#include "./Transform/sTransform.h"
#include "./Helper/cString.h"
#include "./Helper/cMath.h"

cModel::cModel()
{
	_buffer = make_shared<cModelBoneBuffer>();
}

cModel::~cModel()
{
	_materials.clear();
	_bones.clear();
	_meshes.clear();
	_buffer = nullptr;
}

std::unique_ptr<cModel> cModel::Clone() const
{
	struct make_unique_enabler : public cModel {};
	auto model = make_unique<make_unique_enabler>();
	
	model->_modelPath = _modelPath;

	for (auto&& material : _materials)
		model->_materials.push_back(material->Clone());

	for (auto&& bone : _bones)
		model->_bones.push_back(bone->Clone());

	model->_skinnedTM.assign(_skinnedTM.size(), cMath::MATRIX_IDENTITY);

	for (auto&& mesh : _meshes)
		model->_meshes.push_back(mesh->Clone());

	model->_rootBone = model->GetRoot();

	return move(model);
}

void cModel::AnimateBone(UINT index, const D3DXMATRIX& pAnimated)
{
	auto rootTMPtr = _rootTransform.lock();

	int parentIndex = _bones[index]->GetParentIndex();

	_bones[index]->Animate(pAnimated, &rootTMPtr->Matrix);
}

void cModel::ResetBones()
{
	for (auto&& bone : _bones)
		*bone->_animatedTransform = *bone->_absoluteTransform * *_rootTransform.lock();
}

void cModel::Update(weak_ptr<sTransform> rootTransform)
{
	_rootTransform = rootTransform;

	UINT size = _bones.size();
	for (UINT i = 0; i < size; i++)
	{
		_skinnedTM[i] = _bones[i]->GetSkinnedTransform().lock()->Matrix;
	}

	_buffer->SetBones(&_skinnedTM[0], _skinnedTM.size());
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

const vector<shared_ptr<cModelBone>>& cModel::GetBones() const
{
	return _bones;
}

weak_ptr<cModelBone> cModel::GetBone(const wstring & name) const
{
	for (auto&& bone : _bones)
	{
		if (bone->GetName() == name)
			return bone;
	}

	return weak_ptr<cModelBone>();
}

void cModel::SetPlayedBuffer(bool isPlayAnim)
{
	_buffer->SetIsPlayAnim(isPlayAnim);
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