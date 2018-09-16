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

	//스킨 트랜스폼 초기 값은 앱솔루트 트랜스폼 값과 동일하게
	_skinnedTransform = make_unique<sTransform>();
}

cModelBone::~cModelBone()
{
}

void cModelBone::Animate(const D3DXMATRIX & animated, const D3DXMATRIX* root)
{
	if (_parentIndex < 0)
		_animatedTransform->Matrix = animated * *root;
	else
		_animatedTransform->Matrix = animated * _parent.lock()->_animatedTransform->Matrix;

	_skinnedTransform->Matrix = _invAbsoluteMatrix * _animatedTransform->Matrix;
}

vector<weak_ptr<cModelBone>>& cModelBone::GetChildren()
{
	return _children;
}

weak_ptr<sTransform> cModelBone::GetAbsoluteTransform() const
{
	return _absoluteTransform;
}

const D3DXMATRIX & cModelBone::GetInvAbsoluteMatrix() const
{
	return _invAbsoluteMatrix;
}

weak_ptr<sTransform> cModelBone::GetAnimatedTransform() const
{
	return _animatedTransform;
}

const D3DXMATRIX & cModelBone::GetSkinnedMatrix() const
{
	return _skinnedTransform->Matrix;
}

const vector<shared_ptr<cCollider>>& cModelBone::GetColliders() const
{
	return _colliders;
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

	//fixme : 데이터 추가

	return move(clone);
}
