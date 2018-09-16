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
	, _direction(eDirection::FRONT)
	, _distance(0.0f)
	, _rotationSpeed(0.0f)
{
	auto actorPtr = _actor.lock();
	if (actorPtr)
	{
		_transform = actorPtr->GetTransform();
		_blackboard = actorPtr->GetBlackboard();
	}
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
	clone->_direction = _direction;
	clone->_distance = _distance;
	clone->_rotationSpeed = _rotationSpeed;

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
				Move();
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
	{
		if (_type != eMovingType::TO_TARGET)
		{
			const char* items[] = { "Front", "Target Reverse Dir" };

			int type = static_cast<int>(_direction);
			ImGui::Combo("", &type, items, IM_ARRAYSIZE(items));
			_direction = static_cast<eDirection>(type);
		}
	}
	ImGui::PopID();

	ImGui::PushID(3);
	{
		if (_type == eMovingType::CONSTANT || _type == eMovingType::TO_TARGET)
		ImGui::InputFloat("", &_distance);
	}
	ImGui::PopID();

	ImGui::PushID(4);
	{
		if (_type == eMovingType::TO_TARGET)
		{
			ImGui::Text("Rotation Speed :");
			ImGui::InputFloat("", &_rotationSpeed);
		}
	}
	ImGui::PopID();
}

void cMovingAction::LoadJson(Json::Value & root)
{
	LoadDefaultInfo(root);

	Json::GetValue(root, "Speed", _speed);

	int type;
	Json::GetValue(root, "Moving Type", type);
	_type = static_cast<eMovingType>(type);

	Json::GetValue(root, "Direction", type);
	_direction = static_cast<eDirection>(type);

	Json::GetValue(root, "Distance", _distance);
	Json::GetValue(root, "Rotation Speed", _rotationSpeed);
}

void cMovingAction::SaveJson(Json::Value & root)
{
	Json::Value task;

	SaveDefaultInfo(task);

	Json::SetValue(task, "Speed", _speed);

	int type = static_cast<int>(_type);
	Json::SetValue(task, "Moving Type", type);

	type = static_cast<int>(_direction);
	Json::SetValue(task, "Direction", type);

	Json::SetValue(task, "Distance", _distance);
	Json::SetValue(task, "Rotation Speed", _rotationSpeed);

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

	auto boardPtr = _blackboard.lock();
	
	//타겟과 가까워지면 success
	if (boardPtr->GetFloat("DistanceGap") <= _distance)
	{
		_state = eState::SUCCESS;
		return;
	}

	//타겟 방향 가져옴
	const auto& targetDir = boardPtr->GetVector3("TargetDirection");

	//캐릭터 방향 가져옴
	D3DXVECTOR3 actorDir;
	transformPtr->GetDirection(actorDir);

	//둘간의 방향 차이만큼 회전
	RotateAndMove(transformPtr, actorDir, targetDir);

	_state = eState::RUNNING;
}

void cMovingAction::RotateAndMove(OUT shared_ptr<sTransform>& transformPtr, const D3DXVECTOR3 & actorDirection, const D3DXVECTOR3 & targetDirection)
{
	//변환 방향과 캐릭터 방향간 각도 차 계산
	float angle = cMath::AngleBetweenTwoVectors(actorDirection,
												targetDirection);

	D3DXVECTOR3 temp;
	D3DXVec3Cross(&temp, &actorDirection, &targetDirection);
	if (temp.y < 0)
		angle = -angle;

	float speed = angle * _rotationSpeed;

	if (angle > 0)
		transformPtr->Rotate(D3DXVECTOR3{ 0, speed * cFrame::Delta(), 0 });
	else
		transformPtr->Rotate(D3DXVECTOR3{ 0, speed * cFrame::Delta(), 0 });

	transformPtr->Move(_speed);
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
		Move();

		auto transformPtr = _transform.lock();
		transformPtr->Move(_speed);

		_state = eState::RUNNING;
	}
}

void cMovingAction::Move()
{
	auto transformPtr = _transform.lock();

	switch (_direction)
	{
		case cMovingAction::FRONT:
			transformPtr->Move(_speed);
			break;
		case cMovingAction::TARGET_REVERSE:
		{
			auto boardPtr = _blackboard.lock();
			auto dir = boardPtr->GetVector3("TargetDirection");
			D3DXVec3Normalize(&dir, &dir);
			transformPtr->Move(_speed, -dir);
			break;
		}
	}
}
