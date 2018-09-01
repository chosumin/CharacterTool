#include "stdafx.h"
#include "Actions.h"
#include "./BehaviorTree/cBehaviorTree.h"
#include "./GameObject/cActor.h"
#include "./Animator/cAnimator.h"
#include "./Animator/cAnimClip.h"
#include "./Helper/cString.h"
#include "./Blackboard/cBlackboard.h"
#include "./Transform/sTransform.h"
#include "./Viewer/cCameraFactory.h"

cSingleAnimation::cSingleAnimation(weak_ptr<cActor> actor,
							   weak_ptr<cBehaviorTree> tree,
							   const ImVec2& position,
							   weak_ptr<cTask> parent)
	: cTask(tree, "Single Animation", position, parent)
	, _actor(actor)
	, _selectedClip("")
	, _overwrite(false)
{
	
}

cSingleAnimation::~cSingleAnimation()
{
}

std::unique_ptr<cTask> cSingleAnimation::Clone() const
{
	auto task = make_unique<cSingleAnimation>(_actor, _tree, _pos);

	task->_clip = _clip;
	task->_selectedClip = _selectedClip;

	return move(task);
}

cTask::eState cSingleAnimation::Run()
{
	auto actorPtr = _actor.lock();
	if (!actorPtr)
		return eState::FAILURE;

	auto animatorPtr = actorPtr->GetAnimator().lock();

	if (_state == eState::NONE)
	{
		animatorPtr->SetCurrentClip(_clip, _overwrite);
		//hack : 위치가 여기 맞으려나
		_tree.lock()->SetCurrentTask(shared_from_this());
		_state = eState::RUNNING;
	}
	else if (_state == eState::RUNNING)
	{
		//todo : 이벤트 프레임 도달시 이벤트 실행
		if (animatorPtr->IsEndAnimation())
		{
			_state = eState::SUCCESS;
		}
		//todo : 애니메이션 완료시 석세스, 미완료시 Running 
	}

	return _state;
}

void cSingleAnimation::RenderInfo()
{
	auto actorPtr = _actor.lock();
	auto clips = actorPtr->GetAnimator().lock()->GetClips();

	ImGui::TextColored(ImVec4(250, 250, 0, 1), _taskName.c_str());

	ImGui::NewLine();

	if(ImGui::BeginCombo("", _selectedClip.c_str()))
	{
		int i = 0;
		for (auto&& clip : clips)
		{
			ImGui::PushID(i++);

			string clipName = cString::String(clip->GetName());
			bool isSelected = _selectedClip == clipName;
			if (ImGui::Selectable(clipName.c_str(), isSelected))
			{
				_selectedClip = clipName;
				_clip = clip;
			}
			ImGui::SetItemDefaultFocus();

			ImGui::PopID();
		}

		ImGui::EndCombo();
	}

	ImGui::Checkbox("Overwrite", &_overwrite);
}

/*******************************************************************/

cMovingAction::cMovingAction(weak_ptr<cActor> actor, weak_ptr<cBehaviorTree> tree, const ImVec2 & position, weak_ptr<cTask> parent)
	: cTask(tree, "Move", position, parent)
	, _actor(actor)
	, _speed(0.0f)
	, _target(false)
{
}

cMovingAction::~cMovingAction()
{
}

std::unique_ptr<cTask> cMovingAction::Clone() const
{
	auto clone = make_unique<cMovingAction>(_actor, _tree, _pos);
	clone->_actor = _actor;
	clone->_speed = _speed;

	return move(clone);
}

cTask::eState cMovingAction::Run()
{
	auto actorPtr = _actor.lock();
	if (!actorPtr)
		return eState::FAILURE;

	if (_state == eState::NONE)
	{
		_tree.lock()->SetCurrentTask(shared_from_this());
		_state = eState::RUNNING;
	}
	else if (_state == eState::RUNNING)
	{
		auto actor = _actor.lock();
		
		//블랙보드에서 방향 가져옴
		auto blackboard = actor->GetBlackboard().lock();
		
		D3DXVECTOR3 direction;
		blackboard->GetDirection(direction);

		//카메라 방향으로 변환
		
		//todo : 캐릭터의 방향과 비교
		auto transform = actor->GetTransform().lock();
		transform->Move(_speed, direction);

		//타겟 없으면 한 틱만 진행
		if (_target)
			_state = eState::SUCCESS;
	}

	return _state;
}

void cMovingAction::RenderInfo()
{
	auto actorPtr = _actor.lock();
	auto clips = actorPtr->GetAnimator().lock()->GetClips();

	ImGui::TextColored(ImVec4(250, 250, 0, 1), _taskName.c_str());

	ImGui::NewLine();

	ImGui::PushID(0);
	ImGui::Text("Speed");
	ImGui::InputFloat("", &_speed);
	ImGui::PopID();

	ImGui::Checkbox("Target", &_target);
}
