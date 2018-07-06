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

void cActor::Damage()
{
	if (_anim->GetClip() != 8)
		_anim->SetClip(8);

	//hack : 애니메이션 끝나면 idle로 바꾸기
	if (timeGetTime() - time > 800)
	{
		_anim->SetClip(0);
		isDamage = false;
	}
}

weak_ptr<cCollider> cActor::GetCollider(UINT num) const
{
	return _colliders[num];
}

void cActor::PostRender(int i)
{
	//hack : ㅠㅠ
	ImGui::Begin("Inspector");
	{
		ImGui::Separator();

		auto damagedCol = _colliders[0]->GetLocalTransform();
		ControlTransform(i + "damage", damagedCol);

		ImGui::Separator();

		auto attackCol = _colliders[1]->GetLocalTransform();
		ControlTransform(i + "attack", attackCol);

		ImGui::Separator();
	}
	ImGui::End();
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
{
}

