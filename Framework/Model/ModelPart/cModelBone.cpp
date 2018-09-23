#include "stdafx.h"
#include "cModelBone.h"
#include "./Transform/sTransform.h"
#include "./UI/Gizmo/cGizmo.h"
#include "./Helper/cString.h"
#include "./Helper/cMath.h"

cModelBone::cModelBone()
{
	_absoluteTransform = make_shared<sTransform>();

	_animatedTransform = make_unique<sTransform>();

	//��Ų Ʈ������ �ʱ� ���� �ۼַ�Ʈ Ʈ������ ���� �����ϰ�
	_skinnedTransform = make_unique<sTransform>();
}

cModelBone::~cModelBone()
{
}

void cModelBone::Animate(const D3DXMATRIX & animated, const D3DXMATRIX* root)
{
	if (_parentIndex < 0)
	{
		D3DXMatrixMultiply(&_animatedTransform->Matrix, &animated, root);
	}
	else
	{
		D3DXMatrixMultiply(&_animatedTransform->Matrix, &animated, &_parent.lock()->_animatedTransform->Matrix);
	}

	D3DXMatrixMultiply(&_skinnedTransform->Matrix, &_invAbsoluteMatrix, &_animatedTransform->Matrix);
}

const D3DXMATRIX & cModelBone::GetSkinnedMatrix() const
{
	return _skinnedTransform->Matrix;
}

void cModelBone::AddCollider(weak_ptr<cCollider> collider)
{
	_colliders.emplace_back(collider.lock());
}

void cModelBone::DeleteCollider(weak_ptr<cCollider> collider)
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

unique_ptr<cModelBone> cModelBone::Clone() const
{
	auto clone = make_unique<cModelBone>();
	clone->_name = _name;
	clone->_index = _index;
	clone->_parentIndex = _parentIndex;

	//fixme : ������ �߰�

	return move(clone);
}
