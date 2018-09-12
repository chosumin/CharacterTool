#include "stdafx.h"
#include "cMovingAction.h"
#include "./BehaviorTree/cBehaviorTree.h"
#include "./GameObject/cActor.h"
#include "./Blackboard/cBlackboard.h"
#include "./Transform/sTransform.h"
#include "./Viewer/cCameraFactory.h"
#include "./Helper/cMath.h"

cMovingAction::cMovingAction(const weak_ptr<cActor> & actor,
							 const weak_ptr<cBehaviorTree> & tree,
							 const ImVec2 & position,
							 weak_ptr<cTask> parent)
	: cTask(tree, "Move", position, parent)
	, _actor(actor)
	, _speed(0.0f)
	, _type(eMovingType::CONTINUE)
	, _distance(0.0f)
{
	auto actorPtr = _actor.lock();
	if (actorPtr)
		_transform = actorPtr->GetTransform();

}

cMovingAction::~cMovingAction()
{
}

std::unique_ptr<cTask> cMovingAction::Clone() const
{
	auto clone = make_unique<cMovingAction>(_actor, _tree, _pos);
	clone->_actor = _actor;
	clone->_transform = _transform;
	clone->_speed = _speed;
	clone->_type = _type;
	clone->_distance = _distance;

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

		if (_type == eMovingType::CONSTANT)
			_remaining = _distance;
	}

	if (_state == eState::RUNNING)
	{
		switch (_type)
		{
			case cMovingAction::CONTINUE:
			{
				auto transformPtr = _transform.lock();
				transformPtr->Move(_speed);
			}
			break;
			case cMovingAction::TO_TARGET:
				MoveToTarget();
				break;
			case cMovingAction::CONSTANT:
				MoveContantDistance();
				break;
		}
	}

	return _state;
}

void cMovingAction::RenderInfo()
{
	auto actorPtr = _actor.lock();

	ImGui::TextColored(ActionTextColor, _taskName.c_str());

	ImGui::NewLine();

	ImGui::PushID(0);
	{
		ImGui::Text("Speed");
		ImGui::InputFloat("", &_speed);
	}
	ImGui::PopID();

	ImGui::PushID(1);
	{
		ImGui::Text("Moving Type :");

		const char* items[] = { "Continue", "To target", "Constant" };
		int type = static_cast<int>(_type);
		ImGui::Combo("", &type, items, IM_ARRAYSIZE(items));
		_type = static_cast<eMovingType>(type);
	}
	ImGui::PopID();

	ImGui::PushID(2);
	if (_type == eMovingType::CONSTANT)
		ImGui::InputFloat("", &_distance);
	ImGui::PopID();
}

void cMovingAction::LoadJson(Json::Value & root)
{
	LoadDefaultInfo(root);

	Json::GetValue(root, "Speed", _speed);

	int type;
	Json::GetValue(root, "Moving Type", type);
	_type = static_cast<eMovingType>(type);

	Json::GetValue(root, "Distance", _distance);
}

void cMovingAction::SaveJson(Json::Value & root)
{
	Json::Value task;

	SaveDefaultInfo(task);

	Json::SetValue(task, "Speed", _speed);

	int type = static_cast<int>(_type);
	Json::SetValue(task, "Moving Type", type);

	Json::SetValue(task, "Distance", _distance);

	root.append(task);
}

void cMovingAction::MoveToTarget()
{
	auto transformPtr = _transform.lock();
	if (!transformPtr)
	{
		_state = eState::FAILURE;
		return;
	}

	//todo : 타겟 방향으로 이동
	//todo : 아직 이동중이면 running
	//todo : 타겟과 가까워지면 success
}

void cMovingAction::MoveContantDistance()
{
	// 남은거리가 0이면 성공
	if (_remaining <= D3DX_16F_EPSILON)
		_state = eState::SUCCESS;
	else
	{
		_remaining -= _speed * cFrame::Delta();

		//아직 남았으면 러닝
		auto transformPtr = _transform.lock();
		transformPtr->Move(_speed);
		_state = eState::RUNNING;
	}
}
