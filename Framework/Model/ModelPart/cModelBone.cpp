#include "stdafx.h"
#include "cModelBone.h"
#include "./Transform/sTransform.h"
#include "./Transform/cGizmo.h"
#include "./Helper/cString.h"

cModelBone::cModelBone()
{
	//D3DXMatrixIdentity(&_transform);
	_transform = make_shared<sTransform>();
}

cModelBone::~cModelBone()
{
}

void cModelBone::GetTransform(D3DXMATRIX & transform)
{
	_transform->Matrix = transform;
}

D3DXMATRIX & cModelBone::Transform()
{
	return _transform->Matrix;
}

void cModelBone::ShowHierarchy()
{
	auto bOpen = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	if (ImGui::TreeNodeEx(cString::String(_name).c_str(), bOpen))
	{
		for (auto&& bone : _children)
			bone->ShowHierarchy();
		ImGui::TreePop();
	}

	if (ImGui::IsItemClicked())
	{
		//todo : ÀÎ½ºÆåÅÍ ¶ç¿ì±â
		cGizmo::Get()->AddTransform(_transform);
	}
}

unique_ptr<cModelBone> cModelBone::Clone() const
{
	auto clone = make_unique<cModelBone>();
	clone->_name = _name;
	clone->_index = _index;
	clone->_parentIndex = _parentIndex;
	clone->_transform = _transform;

	return move(clone);
}
