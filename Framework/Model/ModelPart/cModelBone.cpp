#include "stdafx.h"
#include "cModelBone.h"
#include "./Transform/sTransform.h"
#include "./UI/Gizmo/cGizmo.h"
#include "./Helper/cString.h"

cModelBone::cModelBone()
{
	//D3DXMatrixIdentity(&_transform);
	_transform = make_shared<sTransform>();
	_absoluteTransform = make_shared<sTransform>();
}

cModelBone::~cModelBone()
{
}

vector<weak_ptr<cModelBone>>& cModelBone::GetChildren()
{
	return _children;
}

weak_ptr<sTransform> cModelBone::GetAbsoluteTransform()
{
	return _absoluteTransform;
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
