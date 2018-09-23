#include "stdafx.h"
#include "cComboAttackAction.h"
#include "./BehaviorTree/cBehaviorTree.h"
#include "./GameObject/cActor.h"
#include "./Collider/cActorColliders.h"
#include "./Animator/cAnimator.h"
#include "./Animator/cAnimClip.h"
#include "./Blackboard/cBlackboard.h"
#include "./Helper/cString.h"
#include "./Helper/cMath.h"

cComboAttackAction::cComboAttackAction(const weak_ptr<cActor> & actor,
								   const weak_ptr<cBehaviorTree>& tree,
								   const ImVec2& position,
								   weak_ptr<cTask> parent)
	: cTask(tree, "Combo Attack", position, parent)
	, _actor(actor)
	, _selectedClip("")
	, _blendTime(0.2f)
	, _attackNumber(1)
	, _nameComboFrame(L"")
	, _nameCollisionFrame(L"")
	, _succeedCollision(false)
{
}

cComboAttackAction::~cComboAttackAction()
{
}

std::unique_ptr<cTask> cComboAttackAction::Clone() const
{
	auto task = make_unique<cComboAttackAction>(_actor, _tree, _pos);

	task->_clip = _clip;
	task->_blendTime = _blendTime;
	task->_selectedClip = _selectedClip;
	task->_attackNumber = _attackNumber;
	task->_nameComboFrame = _nameComboFrame;
	task->_nameCollisionFrame = _nameCollisionFrame;

	return move(task);
}

cTask::eState cComboAttackAction::Run()
{
	auto actorPtr = _actor.lock();
	if (!actorPtr)
		return eState::FAILURE;

	auto& animatorPtr = actorPtr->GetAnimator();
	auto& blackboardPtr = actorPtr->GetBlackboard();

	//blackboardPtr->SetBool("MeleeAttack", true);
	const int& attackNumber = blackboardPtr->GetInt("AttackNumber");

	//현재 공격 번호가 본인보다 높으면 성공 반환
	if (attackNumber > _attackNumber)
		return _state = eState::SUCCESS;

	const int& attackState = blackboardPtr->GetInt("AttackState");

	//최초 진입
	if (attackState == 1)
	{
		animatorPtr->SetCurrentClip(_clip, false, _blendTime, false);
		_tree.lock()->SetCurrentTask(shared_from_this());
		
		//공격 번호 세팅
		blackboardPtr->SetInt("AttackNumber", _attackNumber);

		//공격방향은 카메라 방향
		SetTargetDirection(blackboardPtr);

		//다음 상태 변환
		blackboardPtr->SetInt("AttackState", 2);
		_state = eState::RUNNING;
	}

	//공격 중
	if (attackState == 2 || attackState == 3)
	{
		auto clipPtr = _clip.lock();
		if (clipPtr)
		{
			//충돌
			Collide(clipPtr, animatorPtr);

			//콤보 가능 상태 전환
			if (animatorPtr->GetCurrentFrameCount() >=
				clipPtr->GetEventFrame(_nameComboFrame))
				blackboardPtr->SetInt("AttackState", 3);
		}

		//애니메이션 끝나면 콤보 종료
		if (animatorPtr->IsEndAnimation())
		{
			EndCombo(blackboardPtr);
			return _state = eState::FAILURE;
		}
	}

	//콤보 성공
	if (attackState == 4)
	{
		//최초 진입 상태로 변경
		blackboardPtr->SetInt("AttackState", 1);
		
		//공격 번호 증가
		int nextCount = (_attackNumber + 1) % blackboardPtr->GetInt("MaxAttackNumber");
		blackboardPtr->SetInt("AttackNumber", nextCount);

		//끝까지 갔으면 다시 0부터 시작
		if (nextCount == 0)
			blackboardPtr->SetInt("AttackState", 0);

		_succeedCollision = false;

		return _state = eState::SUCCESS;
	}

	return _state = eState::RUNNING;
}

void cComboAttackAction::SetTargetDirection(shared_ptr<cBlackboard>& blackboardPtr)
{
	D3DXVECTOR3 camDir;
	sGlobalVariable::MainCamera.lock()->
		GetCameraDirection(camDir);
	camDir.y = 0.0f;
	D3DXVec3Normalize(&camDir, &camDir);
	blackboardPtr->SetVector3("TargetDirection", camDir);
}

void cComboAttackAction::Collide(const shared_ptr<cAnimClip> & clipPtr, const shared_ptr<cAnimator> & animatorPtr)
{
	//충돌 처리를 이미 완료했는지
	bool onEvent = _succeedCollision == false;

	//충돌 체크 프레임인지
	onEvent &= clipPtr->GetEventFrame(_nameCollisionFrame) == animatorPtr->GetCurrentFrameCount();

	if (!onEvent)
		return;

	//충돌 처리
	_actor.lock()->Attack();
}

void cComboAttackAction::EndCombo(shared_ptr<cBlackboard>& blackboardPtr)
{
	blackboardPtr->SetInt("AttackState", 0);
	blackboardPtr->SetBool("MeleeAttack", false);
	blackboardPtr->SetInt("AttackNumber", 0);

	_succeedCollision = false;
}

void cComboAttackAction::RenderInfo()
{
	auto actorPtr = _actor.lock();
	auto& clips = actorPtr->GetAnimator()->GetClips();

	ImGui::TextColored(ActionTextColor, _taskName.c_str());

	ImGui::NewLine();

	int index = 0;
	ImGui::PushID(index++);
	ImGui::Text("Animation");
	if (ImGui::BeginCombo("", _selectedClip.c_str()))
	{
		for (auto&& clip : clips)
		{
			string clipName = cString::String(clip->GetName());
			bool isSelected = _selectedClip == clipName;
			if (ImGui::Selectable(clipName.c_str(), isSelected))
			{
				_selectedClip = clipName;
				_clip = clip;
			}
			ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}
	ImGui::PopID();

	ImGui::PushID(index++);
	{
		ImGui::Text("Blending Time :");
		ImGui::InputFloat("", &_blendTime);
	}
	ImGui::PopID();

	ImGui::PushID(index++);
	{
		ImGui::Text("Attack Number");
		ImGui::InputInt("", &_attackNumber);
	}
	ImGui::PopID();

	EventFrameComboBox(index, "Combo Frame", _nameComboFrame);
	EventFrameComboBox(index, "Collision Frame", _nameCollisionFrame);
}

void cComboAttackAction::LoadJson(Json::Value & root)
{
	LoadDefaultInfo(root);

	Json::GetValue(root, "Clip Name", _selectedClip);

	if (_selectedClip != "")
	{
		auto actorPtr = _actor.lock();
		auto& animPtr = actorPtr->GetAnimator();

		wstring clipName = cString::Wstring(_selectedClip);
		for (auto&& clip : animPtr->GetClips())
		{
			if (clip->GetName() == clipName)
			{
				_clip = clip;
				break;
			}
		}
	}

	Json::GetValue(root, "Blend Time", _blendTime);
	Json::GetValue(root, "Attack Number", _attackNumber);

	string name;
	Json::GetValue(root, "Combo Frame Name", name);
	_nameComboFrame = cString::Wstring(name);

	Json::GetValue(root, "Collision Frame Name", name);
	_nameCollisionFrame = cString::Wstring(name);
}

void cComboAttackAction::SaveJson(Json::Value & root)
{
	Json::Value task;

	SaveDefaultInfo(task);

	string clipName = "";
	auto clipPtr = _clip.lock();
	if (clipPtr)
		clipName = cString::String(clipPtr->GetName());
	Json::SetValue(task, "Clip Name", clipName);

	Json::SetValue(task, "Blend Time", _blendTime);
	Json::SetValue(task, "Attack Number", _attackNumber);

	string name = cString::String(_nameComboFrame);
	Json::SetValue(task, "Combo Frame Name", name);

	name = cString::String(_nameCollisionFrame);
	Json::SetValue(task, "Collision Frame Name", name);

	root.append(task);
}

void cComboAttackAction::EventFrameComboBox(int & index,
									   const string & title,
									   wstring& name)
{
	auto clipPtr = _clip.lock();
	if (!clipPtr)
		return;

	ImGui::PushID(index++);

	ImGui::Text(title.c_str());

	const auto& eventFrames = clipPtr->GetEventFrames();
	string strName = cString::String(name);
	if (ImGui::BeginCombo("", strName.c_str()))
	{
		for (auto&& eventFrame : eventFrames)
		{
			string frameName = cString::String(eventFrame.first);
			bool isSelected = strName == frameName;
			if (ImGui::Selectable(frameName.c_str(), isSelected))
			{
				name = eventFrame.first;
			}
			ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::PopID();
}
