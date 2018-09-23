#include "stdafx.h"
#include "cRotatingAction.h"
#include "./BehaviorTree/cBehaviorTree.h"
#include "./GameObject/cActor.h"
#include "./Blackboard/cBlackboard.h"
#include "./Transform/sTransform.h"
#include "./Viewer/cCameraFactory.h"
#include "./Helper/cMath.h"

cRotatingAction::cRotatingAction(const weak_ptr<cActor> & actor,
								 const weak_ptr<cBehaviorTree>& tree,
								 const ImVec2 & position,
								 weak_ptr<cTask> parent)
	: cTask(tree, "Rotate", position, parent)
	, _actor(actor)
	, _rotationSpeed(10.0f)
	, _rotationType(eRotationType::TARGET)
	, _rotationAngle(0.0f)
	, _maxSpeed(50.0f)
{
	auto actorPtr = _actor.lock();
	if (actorPtr)
		_transform = actorPtr->GetTransform();
}

cRotatingAction::~cRotatingAction()
{
}

std::unique_ptr<cTask> cRotatingAction::Clone() const
{
	auto clone = make_unique<cRotatingAction>(_actor, _tree, _pos);

	clone->_rotationSpeed = _rotationSpeed;
	clone->_rotationType = _rotationType;
	clone->_rotationAngle = _rotationAngle;

	return move(clone);
}

cTask::eState cRotatingAction::Run()
{
	auto actorPtr = _actor.lock();
	if (!actorPtr)
		return eState::FAILURE;

	if (_state == eState::NONE)
	{
		_tree.lock()->SetCurrentTask(shared_from_this());
		_state = eState::RUNNING;

		if (_rotationType == eRotationType::CONSTANT)
			_remaining = _rotationAngle;
	}

	if (_state == eState::RUNNING)
	{
		switch (_rotationType)
		{
			case eRotationType::TARGET:
				RotateTarget();
				break;
			case eRotationType::KEYBOARD:
				RotateKeyboard();
				break;
			case eRotationType::CONSTANT:
				RotateAngle();
				break;
		}
	}

	return _state;
}

void cRotatingAction::RenderInfo()
{
	auto actorPtr = _actor.lock();

	ImGui::TextColored(ActionTextColor, _taskName.c_str());

	ImGui::NewLine();

	ImGui::PushID(0);
	{
		ImGui::Text("Rotation Type :");

		const char* items[] = { "To Target", "User Input", "Constant" };
		int type = static_cast<int>(_rotationType);
		ImGui::Combo("", &type, items, IM_ARRAYSIZE(items));
		_rotationType = static_cast<eRotationType>(type);
	}
	ImGui::PopID();

	ImGui::PushID(1);
	{
		if (_rotationType == eRotationType::CONSTANT)
		{
			ImGui::Text("Angle :");
			ImGui::InputFloat("", &_rotationAngle);
		}
	}
	ImGui::PopID();

	ImGui::PushID(2);
	{
		ImGui::Text("Rotation Speed :");
		ImGui::InputFloat("", &_rotationSpeed);
	}
	ImGui::PopID();
}

void cRotatingAction::LoadJson(Json::Value & root)
{
	LoadDefaultInfo(root);

	Json::GetValue(root, "Rotation Speed", _rotationSpeed);

	int type;
	Json::GetValue(root, "Rotation Type", type);
	_rotationType = static_cast<eRotationType>(type);

	Json::GetValue(root, "Rotation Angle", _rotationAngle);
}

void cRotatingAction::SaveJson(Json::Value & root)
{
	Json::Value task;

	SaveDefaultInfo(task);

	Json::SetValue(task, "Rotation Speed", _rotationSpeed);

	int type = static_cast<int>(_rotationType);
	Json::SetValue(task, "Rotation Type", type);

	Json::SetValue(task, "Rotation Angle", _rotationAngle);

	root.append(task);
}

void cRotatingAction::RotateTarget()
{
	auto actorPtr = _actor.lock();
	auto& blackboardPtr = actorPtr->GetBlackboard();

	if (!blackboardPtr)
		return;

	//타겟 방향 가져옴
	const auto& targetDir = blackboardPtr->GetVector3("TargetDirection");

	//캐릭터 방향 가져옴
	auto transformPtr = _transform.lock();
	D3DXVECTOR3 actorDir;
	transformPtr->GetDirection(actorDir);

	//둘간의 방향 차이만큼 회전
	Rotate(transformPtr, actorDir, targetDir);
}

void cRotatingAction::RotateKeyboard()
{
	auto actorPtr = _actor.lock();

	//블랙보드에서 방향 가져옴
	auto& blackboard = actorPtr->GetBlackboard();
	auto direction = blackboard->GetVector3("Direction");

	//카메라 행렬로 변환된 방향 계산
	D3DXMATRIX rotation;
	D3DXVECTOR2 radians;
	sGlobalVariable::MainCamera.lock()->GetRotation(&radians);
	D3DXMatrixRotationY(&rotation, radians.y);

	D3DXVec3TransformNormal(&direction, &direction, &rotation);

	//캐릭터 방향 계산
	auto transformPtr = _transform.lock();
	D3DXVECTOR3 actorDirection;
	transformPtr->GetDirection(actorDirection);

	Rotate(transformPtr, actorDirection, direction);
}

void cRotatingAction::RotateAngle()
{
	//캐릭터 위치 가져옴
	auto transformPtr = _transform.lock();
	if (!transformPtr)
	{
		_state = eState::FAILURE;
		return;
	}

	//각도만큼 회전
	if (abs(_remaining) < D3DX_16F_EPSILON)
	{
		_state = eState::SUCCESS;
	}
	else
	{
		//최대 속도이면 바로 성공 반환
		if (_rotationSpeed >= _maxSpeed)
		{
			transformPtr->Rotate(D3DXVECTOR3{ 0, _remaining, 0 });
			_state = eState::SUCCESS;
			return;
		}

		if (_remaining > 0)
		{
			_remaining += _rotationSpeed * cFrame::Delta();
			transformPtr->Rotate(D3DXVECTOR3{ 0, _rotationSpeed * cFrame::Delta(), 0 });
		}
		else
		{
			_remaining -= _rotationSpeed * cFrame::Delta();
			transformPtr->Rotate(D3DXVECTOR3{ 0, -_rotationSpeed * cFrame::Delta(), 0 });
		}
		_state = eState::RUNNING;
	}
}

void cRotatingAction::Rotate(OUT shared_ptr<sTransform> & transformPtr,
							 const D3DXVECTOR3& actorDirection,
							 const D3DXVECTOR3& targetDirection)
{
	//변환 방향과 캐릭터 방향간 각도 차 계산
	float angle = cMath::AngleBetweenTwoVectors(actorDirection,
												targetDirection);

	//회전 완료됐으면 성공 반환
	if (angle <= 5.0f)
	{
		_state = eState::SUCCESS;
		return;
	}

	D3DXVECTOR3 temp;
	D3DXVec3Cross(&temp, &actorDirection, &targetDirection);
	if (temp.y < 0)
		angle = -angle;

	//최대 속도이면 바로 성공 반환
	if (_rotationSpeed >= _maxSpeed)
	{
		transformPtr->Rotate(D3DXVECTOR3{ 0, angle, 0 });
		_state = eState::SUCCESS;
		return;
	}

	float speed = angle * _rotationSpeed;

	if (angle > 0)
		transformPtr->Rotate(D3DXVECTOR3{ 0, speed * cFrame::Delta(), 0 });
	else
		transformPtr->Rotate(D3DXVECTOR3{ 0, speed * cFrame::Delta(), 0 });

	_state = eState::RUNNING;
}
