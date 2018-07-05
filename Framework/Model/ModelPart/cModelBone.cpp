#include "stdafx.h"
#include "cModelBone.h"
#include "./Transform/sTransform.h"
#include "./UI/Inspector/Transform/cGizmo.h"
#include "./Helper/cString.h"

cModelBone::cModelBone()
{
	//D3DXMatrixIdentity(&_transform);
	_transform = make_shared<sTransform>();
	_absoluteTransform = make_shared<sTransform>();

	//test : 콜라이더 부모 테스트
	_matrix = make_shared<sTransform>();
}

cModelBone::~cModelBone()
{
}

void cModelBone::GetTransform(D3DXMATRIX & transform)
{
	_transform->Matrix = transform;
}

void cModelBone::AbsoluteTransform(D3DXMATRIX & transform)
{
	_absoluteTransform->Matrix = transform;
}

D3DXMATRIX & cModelBone::Transform()
{
	return _transform->Matrix;
}

D3DXMATRIX & cModelBone::AbsoluteTransform()
{
	return _absoluteTransform->Matrix;
}

void cModelBone::ShowHierarchy()
{
	ImGuiTreeNodeFlags bOpen = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
	if (ImGui::TreeNodeEx(to_string(_index).c_str(), bOpen))
	{
		ImGui::SameLine();
		if (ImGui::Selectable(cString::String(_name).c_str()))
			cGizmo::Get()->AddTransform(_absoluteTransform);

		for (auto&& bone : _children)
			bone->ShowHierarchy();
		ImGui::TreePop();
	}
}

weak_ptr<struct sTransform> cModelBone::GetTransform() const
{
	return _matrix;
}

void cModelBone::SetTransform(D3DXMATRIX & matrix)
{
	_matrix->Matrix = matrix;
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
