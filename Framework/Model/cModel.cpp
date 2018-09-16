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

ID3D11RasterizerState* cModel::_rasterizer[2];
bool cModel::_init = false;

cModel::cModel()
{
	_buffer = make_shared<cModelBoneBuffer>();

	if (_init == false)
	{
		D3D11_RASTERIZER_DESC desc;
		cStates::GetRasterizerDesc(&desc);
		cStates::CreateRasterizer(&desc, &_rasterizer[0]);

		//desc.CullMode = D3D11_CULL_FRONT;
		cStates::CreateRasterizer(&desc, &_rasterizer[1]);
		_init = true;
	}
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
	if (index == 0)
	{
		auto rootTMPtr = _rootTransform.lock();
		_bones[index]->Animate(pAnimated, &rootTMPtr->Matrix);
	}
	else
		_bones[index]->Animate(pAnimated, nullptr);
}

void cModel::ResetBones()
{
	sTransform root;
	for (auto&& bone : _bones)
	{
		if (!_rootTransform.expired())
			*bone->_animatedTransform = *bone->_absoluteTransform * *_rootTransform.lock();
		else
			*bone->_animatedTransform = *bone->_absoluteTransform * root;
	}
}

void cModel::Update(const weak_ptr<sTransform> & rootTransform)
{
	_rootTransform = rootTransform;

	UINT size = _bones.size();
	for (UINT i = 0; i < size; i++)
	{
		memcpy(&_skinnedTM[i], &_bones[i]->GetSkinnedMatrix(), sizeof(D3DXMATRIX));
	}

	_buffer->SetBones(&_skinnedTM[0], _skinnedTM.size());
}

void cModel::Render()
{
	_buffer->SetVSBuffer(2);
	for (auto&& mesh : _meshes)
	{
		if (mesh->GetName() == L"Cape_Mesh")
		{
			D3D::GetDC()->RSSetState(_rasterizer[1]);
			mesh->Render();
			D3D::GetDC()->RSSetState(nullptr);
		}
		else
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