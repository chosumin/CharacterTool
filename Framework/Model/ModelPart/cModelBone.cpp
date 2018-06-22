#include "stdafx.h"
#include "cModelBone.h"

cModelBone::cModelBone()
{
	D3DXMatrixIdentity(&_transform);
}

cModelBone::~cModelBone()
{
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
