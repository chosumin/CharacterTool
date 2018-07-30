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

std::unique_ptr<cModel> cModel::Clone() const
{
	struct make_unique_enabler : public cModel {};
	auto model = make_unique<make_unique_enabler>();
	
	for (auto&& material : _materials)
		model->_materials.push_back(material->Clone());

	for (auto&& bone : _originalBones)
		model->_originalBones.push_back(bone->Clone());

	model->_animatedTM.assign(_animatedTM.size(), cMath::MATRIX_IDENTITY);
	model->_skinnedTM.assign(_skinnedTM.size(), cMath::MATRIX_IDENTITY);

	for (auto&& mesh : _meshes)
		model->_meshes.push_back(mesh->Clone());

	model->_rootBone = model->GetRoot();

	return move(model);
}

void cModel::AnimateBone(UINT index, const D3DXMATRIX& pAnimated)
{
	auto rootTMPtr = _rootTransform.lock();

	int parentIndex = _originalBones[index]->GetParentIndex();

	if (parentIndex < 0)
		_animatedTM[index] = pAnimated * rootTMPtr->Matrix;
	else
		_animatedTM[index] = pAnimated * _animatedTM[parentIndex];
}

void cModel::UpdateWorld()
{
	auto rootTMPtr = _rootTransform.lock();
	UINT size = _originalBones.size();
	for (UINT i = 0; i < size; i++)
	{
		int parentIndex = _originalBones[i]->GetParentIndex();

		if (parentIndex < 0)
		{
			if (rootTMPtr)
				_animatedTM[i] = _originalBones[i]->_transform->Matrix * rootTMPtr->Matrix;
			else
				_animatedTM[i] = cMath::MATRIX_IDENTITY;
		}
		else
			_animatedTM[i] = _originalBones[i]->_transform->Matrix * _animatedTM[parentIndex];
	}
}

cModel::~cModel()
{
	_materials.clear();
	_originalBones.clear();
	_meshes.clear();
	_buffer = nullptr;
}

void cModel::Update(weak_ptr<sTransform> rootTransform)
{
	_rootTransform = rootTransform;

	UINT size = _originalBones.size();
	for (UINT i = 0; i < size; i++)
	{
		_skinnedTM[i] = _originalBones[i]->_transform->Matrix * _animatedTM[i];
		//_skinnedTM[i] = _originalBones[i]->_transform->Matrix;
	}
	
	//todo : 처음부터 버퍼의 행렬을 조작한 뒤 쉐이더에서는 전치를 하여 사용한다.
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
	return _originalBones;
}

void cModel::CopyAbsoluteBoneTo(vector<D3DXMATRIX>& transforms)
{
	transforms.clear();
	transforms.assign(_originalBones.size(), D3DXMATRIX());

	for (size_t i = 0; i < _originalBones.size(); i++)
	{
		auto bone = _originalBones[i];

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