#include "stdafx.h"
#include "cSingleAttackAction.h"
#include "./BehaviorTree/cBehaviorTree.h"
#include "./GameObject/cActor.h"
#include "./Collider/cActorColliders.h"
#include "./Animator/cAnimator.h"
#include "./Animator/cAnimClip.h"
#include "./Blackboard/cBlackboard.h"
#include "./Helper/cString.h"
#include "./Helper/cMath.h"

cSingleAttackAction::cSingleAttackAction(const weak_ptr<cActor> & actor,
									   const weak_ptr<cBehaviorTree>& tree,
									   const ImVec2& position,
									   weak_ptr<cTask> parent)
	: cTask(tree, "Single Attack", position, parent)
	, _actor(actor)
	, _selectedClip("")
	, _blendTime(0.2f)
	, _nameCollisionFrame(L"")
	, _succeedCollision(false)
{
}

cSingleAttackAction::~cSingleAttackAction()
{
}

std::unique_ptr<cTask> cSingleAttackAction::Clone() const
{
	auto task = make_unique<cSingleAttackAction>(_actor, _tree, _pos);

	task->_clip = _clip;
	task->_blendTime = _blendTime;
	task->_selectedClip = _selectedClip;
	task->_nameCollisionFrame = _nameCollisionFrame;

	return move(task);
}

cTask::eState cSingleAttackAction::Run()
{
	auto actorPtr = _actor.lock();
	if (!actorPtr)
		return _state = eState::FAILURE;

	auto animatorPtr = actorPtr->GetAnimator().lock();

	//최초 진입
	if (_state == eState::NONE)
	{
		_succeedCollision = false;

		animatorPtr->SetCurrentClip(_clip, false, _blendTime, false);
		_tree.lock()->SetCurrentTask(shared_from_this());

		_state = eState::RUNNING;
	}

	//공격 중
	if (_state == eState::RUNNING)
	{
		auto clipPtr = _clip.lock();
		if (clipPtr)
		{
			//충돌
			Collide(clipPtr, animatorPtr);
		}

		//애니메이션 끝나면 콤보 종료
		if (animatorPtr->IsEndAnimation())
		{
			return _state = eState::SUCCESS;
		}
	}

	return _state;
}

void cSingleAttackAction::Collide(const shared_ptr<cAnimClip> & clipPtr, const shared_ptr<cAnimator> & animatorPtr)
{
	//충돌 처리를 이미 완료했는지
	bool onEvent = _succeedCollision == false;

	//충돌 체크 프레임인지
	onEvent &= clipPtr->GetEventFrame(_nameCollisionFrame) == animatorPtr->GetCurrentFrameCount();

	if (!onEvent)
		return;

	auto colliders = _actor.lock()->GetColliders().lock();

	//충돌 처리
	_actor.lock()->Attack();
}

void cSingleAttackAction::RenderInfo()
{
	auto actorPtr = _actor.lock();
	auto clips = actorPtr->GetAnimator().lock()->GetClips();

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

	EventFrameComboBox(index, "Collision Frame", _nameCollisionFrame);
}

void cSingleAttackAction::LoadJson(Json::Value & root)
{
	LoadDefaultInfo(root);

	Json::GetValue(root, "Clip Name", _selectedClip);

	if (_selectedClip != "")
	{
		auto actorPtr = _actor.lock();
		auto animPtr = actorPtr->GetAnimator().lock();

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

	string name;
	Json::GetValue(root, "Collision Frame Name", name);
	_nameCollisionFrame = cString::Wstring(name);
}

void cSingleAttackAction::SaveJson(Json::Value & root)
{
	Json::Value task;

	SaveDefaultInfo(task);

	string clipName = "";
	auto clipPtr = _clip.lock();
	if (clipPtr)
		clipName = cString::String(clipPtr->GetName());
	Json::SetValue(task, "Clip Name", clipName);

	Json::SetValue(task, "Blend Time", _blendTime);

	string name = cString::String(_nameCollisionFrame);
	Json::SetValue(task, "Collision Frame Name", name);

	root.append(task);
}

void cSingleAttackAction::EventFrameComboBox(int & index,
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
