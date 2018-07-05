#include "stdafx.h"
#include "cActor.h"
#include "./Model/cModel.h"
#include "./Transform/sTransform.h"
#include "./Model/cModelAnimPlayer.h"
#include "./BehaviorTree/cBehaviorTree.h"
#include "./Collider/cCollider.h"

cActor::~cActor()
{
}

void cActor::Update()
{
	_transform->Update();
	_behaviorTree->Update();
	_anim->Update(_transform);
	for (auto&& col : _colliders)
		col->Update();
}

void cActor::Render()
{
	_anim->Render();
	for (auto&& col : _colliders)
		col->Render();
}

weak_ptr<struct sTransform> cActor::GetTransform() const
{
	return _transform;
}

void cActor::GetAction(eAction actionType)
{
	if(_actions[actionType])
		_actions[actionType]();
}

void cActor::SetAction(eAction actionType, function<void()> func)
{
}

UINT cActor::GetCurrentAnim() const
{
	return _anim->GetClip();
}

void cActor::Idle()
{
	if (_anim->GetClip() != 0)
		_anim->SetClip(0);

	isAttack = false;
}

void cActor::Rotate(float deltaY)
{
	//회전
	if(_anim->GetClip() != 1)
		_anim->SetClip(1);
	_transform->Rotate({ 0, deltaY, 0 });
}

void cActor::Move(D3DXVECTOR3 direction, float velocity)
{
	if (velocity >= cFrame::Delta() * 30)
	{
		if (_anim->GetClip() != 2)
			_anim->SetClip(2);
	}
	else
	{
		if (_anim->GetClip() != 1)
			_anim->SetClip(1);
	}

	auto rotate = _transform->GetRotationMatrix();
	D3DXVec3TransformNormal(&direction, &direction, &rotate);
	_transform->Position += direction * velocity;
}

void cActor::Attack()
{
	if (_anim->GetClip() != 6)
		_anim->SetClip(6);

	//hack : 애니메이션 끝나면 idle로 바꾸기
	if (timeGetTime() - time > 1500)
	{
		_anim->SetClip(0);
		isAttack = false;
	}
	//if(_anim->)

	//todo : 캡슐 교차 판정
}

weak_ptr<cCollider> cActor::GetCollider() const
{
	return _colliders[0];
}

cActor::cActor()
{
}

