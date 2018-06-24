#include "stdafx.h"
#include "cModel.h"

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

	model->_root = _root->Clone();

	return move(model);
}

cModel::~cModel()
{
}

void cModel::Update()
{
}

void cModel::Render()
{
	for (auto&& mesh : _meshes)
	{
		mesh->Render();
	}
}

void cModel::ShowHierarchy()
{
	if (ImGui::TreeNode(cString::String(_root->GetName()).c_str()))
	{
		auto bOpen = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		if (ImGui::TreeNodeEx(cString::String(_root->GetName()).c_str(), bOpen))
		{
			_root->ShowHierarchy();
			ImGui::TreePop();
		}

		if(ImGui::IsItemClicked())
		{
			/*cDebug::Log(&typeid(this), []()
			{
				cout << "클릭!" << endl;
			});*/
			//todo : 인스펙터 띄우기
		}
		ImGui::TreePop();
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