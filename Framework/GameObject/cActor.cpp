#include "stdafx.h"
#include "cActor.h"
#include "./Model/cModel.h"
#include "./Transform/sTransform.h"
#include "./BehaviorTree/cBehaviorTree.h"
#include "./Collider/cActorColliders.h"
#include "./Animator/cAnimator.h"

cActor::~cActor()
{
}

void cActor::Init()
{
	_transform = make_shared<sTransform>();
	_colliders = make_shared<cActorColliders>(shared_from_this());
	_animator = make_shared<cAnimator>(_model);
}

void cActor::Update()
{
	if (_transform)
		_transform->Update();
	
	if (_behaviorTree)
		_behaviorTree->Update();

	if (_animator)
		_animator->Update();

	if (_model)
		_model->Update(_transform);

	if (_colliders)
		_colliders->Update();
}

void cActor::Render()
{
	if (_transform)
		_transform->SetVSBuffer(1);

	if (_model)
	{
		_model->SetPlayedBuffer(false);

		if (_animator &&
			_animator->GetMode() != cAnimator::Mode::STOP)
			_model->SetPlayedBuffer(true);

		_model->Render();
	}
		

	//툴 클래스에서 액터 콜라이더 렌더링 처리
	/*if (_colliders)
		_colliders->Render();*/
}

weak_ptr<cModel> cActor::GetModel() const
{
	if(_model)
		return _model;

	return weak_ptr<cModel>();
}

weak_ptr<sTransform> cActor::GetTransform() const
{
	if(_transform)
		return _transform;

	return weak_ptr<sTransform>();
}

weak_ptr<cActorColliders> cActor::GetColliders() const
{
	return _colliders;
}

weak_ptr<cAnimator> cActor::GetAnimator() const
{
	return _animator;
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
	return 0;
}

void cActor::SetModel(weak_ptr<cModel> model)
{
	if (model.expired())
		return;

	_model = model.lock();
}

void cActor::Idle()
{
	/*if (_anim->GetClip() != 0)
		_anim->SetClip(0);

	isAttack = false;*/
}

void cActor::Rotate(float deltaY)
{
	////회전
	//if(_anim->GetClip() != 1)
	//	_anim->SetClip(1);
	//_transform->Rotate({ 0, deltaY, 0 });
}

void cActor::Move(D3DXVECTOR3 direction, float velocity)
{
	/*if (velocity >= cFrame::Delta() * 30)
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
	_transform->Position += direction * velocity;*/
}

void cActor::Attack()
{
	//if (_anim->GetClip() != 6)
	//	_anim->SetClip(6);

	////hack : 애니메이션 끝나면 idle로 바꾸기
	//if (timeGetTime() - time > 1500)
	//{
	//	_anim->SetClip(0);
	//	isAttack = false;
	//}
	//if(_anim->)

	//todo : 캡슐 교차 판정
}

void cActor::Damage()
{
	//if (_anim->GetClip() != 8)
	//	_anim->SetClip(8);

	////hack : 애니메이션 끝나면 idle로 바꾸기
	//if (timeGetTime() - time > 800)
	//{
	//	_anim->SetClip(0);
	//	isDamage = false;
	//}
}

weak_ptr<cCollider> cActor::GetCollider(UINT num) const
{
	return weak_ptr<cCollider>();
}

void cActor::PostRender(int i)
{
}

void cActor::ControlTransform(string name, weak_ptr<struct sTransform> transform)
{
	auto trPtr = transform.lock();
	auto rotate = trPtr->Rotation;
	ImGui::SliderFloat3((name + "Pos").c_str(), trPtr->Position, -100, 100);
	ImGui::SliderFloat3((name + "Sca").c_str(), trPtr->Scaling, -100, 100);
	ImGui::SliderFloat3((name + "Rot").c_str(), trPtr->Rotation, -100, 100);

	trPtr->Rotate(trPtr->Rotation - rotate);
}

cActor::cActor()
	:_name(L"New Actor")
{
}

