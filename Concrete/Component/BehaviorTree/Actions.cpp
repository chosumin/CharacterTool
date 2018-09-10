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
#include "./Helper/cMath.h"

cDataSetter::cDataSetter(const weak_ptr<cActor>& actor,
						 const weak_ptr<cBehaviorTree>& tree,
						 const ImVec2 & position,
						 weak_ptr<cTask> parent)
	: cTask(tree,"Set Blackboard Data", position, parent)
	, _type(eDataType::BOOL)
	, _paramName("")
	, _compareBool(false)
	, _compareFloat(0.0f)
	, _compareVector3(0, 0, 0)
	, _compareInt(0)
{
	_actor = actor;

	auto actorPtr = _actor.lock();
	if (actorPtr)
		_blackboard = actorPtr->GetBlackboard();
}

cDataSetter::~cDataSetter()
{
}

std::unique_ptr<cTask> cDataSetter::Clone() const
{
	auto clone = make_unique<cDataSetter>(_actor, _tree, _pos);

	clone->_actor = _actor;
	clone->_blackboard = _blackboard;
	clone->_type = _type;
	clone->_paramName = _paramName;
	clone->_compareBool = _compareBool;
	clone->_compareFloat = _compareFloat;
	clone->_compareVector3 = _compareVector3;
	clone->_compareInt = _compareInt;

	return move(clone);
}

cTask::eState cDataSetter::Run()
{
	auto blackboardPtr = _blackboard.lock();
	if (!blackboardPtr)
		return _state = eState::FAILURE;

	if (_state == eState::NONE)
	{
		switch (_type)
		{
			case cDataSetter::eDataType::BOOL:
				blackboardPtr->SetBool(_paramName, _compareBool);
				break;
			case cDataSetter::eDataType::FLOAT:
				blackboardPtr->SetFloat(_paramName, _compareFloat);
				break;
			case cDataSetter::eDataType::VECTOR3:
				blackboardPtr->SetVector3(_paramName, _compareVector3);
				break;
			case cDataSetter::eDataType::INT:
				blackboardPtr->SetInt(_paramName, _compareInt);
				break;
		}
	}

	return _state = eState::SUCCESS;
}

void cDataSetter::RenderInfo()
{
	auto blackboardPtr = _blackboard.lock();
	ImGui::TextColored(ActionTextColor, _taskName.c_str());

	ImGui::NewLine();

	ImGui::PushID(0);
	{
		ImGui::Text("Data Type");
		int type = static_cast<int>(_type);
		ImGui::Combo("", &type, "Bool\0Float\0Vector3\0Int");
		_type = static_cast<eDataType>(type);
	}
	ImGui::PopID();

	ImGui::NewLine();

	ImGui::PushID(1);
	{
		ImGui::Text("Blackboard Data :");
		ComboBox(_paramName);
	}
	ImGui::PopID();

	ImGui::Text("Set");

	ImGui::PushID(2);
	{
		switch (_type)
		{
			case cDataSetter::eDataType::BOOL:
			{
				string str = _compareBool ? "TRUE" : "FALSE";
				ImGui::Checkbox(str.c_str(), &_compareBool);
			}
			break;
			case cDataSetter::eDataType::FLOAT:
			{
				ImGui::InputFloat("", &_compareFloat);
			}
			break;
			case cDataSetter::eDataType::VECTOR3:
			{
				ImGui::InputFloat3("", &_compareVector3[0]);
			}
			break;
			case cDataSetter::eDataType::INT:
			{
				ImGui::InputInt("", &_compareInt);
			}
			break;
		}
	}
	ImGui::PopID();
}

void cDataSetter::LoadJson(Json::Value & root)
{
	LoadDefaultInfo(root);

	int type;
	Json::GetValue(root, "Data Type", type);
	_type = static_cast<eDataType>(type);

	Json::GetValue(root, "Value Name", _paramName);
	Json::GetValue(root, "Compare Bool", _compareBool);
	Json::GetValue(root, "Compare Float", _compareFloat);
	Json::GetValue(root, "Compare Vector3", _compareVector3);
	Json::GetValue(root, "Compare Int", _compareInt);
}

void cDataSetter::SaveJson(Json::Value & root)
{
	Json::Value task;

	SaveDefaultInfo(task);

	int type = static_cast<int>(_type);
	Json::SetValue(task, "Data Type", type);

	Json::SetValue(task, "Value Name", _paramName);
	Json::SetValue(task, "Compare Bool", _compareBool);
	Json::SetValue(task, "Compare Float", _compareFloat);
	Json::SetValue(task, "Compare Vector3", _compareVector3);
	Json::SetValue(task, "Compare Int", _compareInt);

	root.append(task);
}

void cDataSetter::ComboBox(string & data)
{
	auto blackboardPtr = _blackboard.lock();
	switch (_type)
	{
		case eDataType::FLOAT: //float
		{
			if (ImGui::BeginCombo("", data.c_str()))
			{
				if (blackboardPtr)
				{
					for (const auto& key : blackboardPtr->GetFloats())
					{
						const string& name = key.first;
						bool isSelected = data == name;
						if (ImGui::Selectable(name.c_str(), isSelected))
							data = name;
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}
		break;
		case eDataType::VECTOR3: //vector3
		{
			if (ImGui::BeginCombo("", data.c_str()))
			{
				if (blackboardPtr)
				{
					for (const auto& key : blackboardPtr->GetVector3s())
					{
						const string& name = key.first;
						bool isSelected = data == name;
						if (ImGui::Selectable(name.c_str(), isSelected))
							data = name;
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}
		break;
		case eDataType::BOOL: //bool
		{
			if (ImGui::BeginCombo("", data.c_str()))
			{
				if (blackboardPtr)
				{
					for (const auto& key : blackboardPtr->GetBools())
					{
						const string& name = key.first;
						bool isSelected = data == name;
						if (ImGui::Selectable(name.c_str(), isSelected))
							data = name;
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}
		break;
		case eDataType::INT: //int
		{
			if (ImGui::BeginCombo("", data.c_str()))
			{
				if (blackboardPtr)
				{
					for (const auto& key : blackboardPtr->GetInts())
					{
						const string& name = key.first;
						bool isSelected = data == name;
						if (ImGui::Selectable(name.c_str(), isSelected))
							data = name;
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
		}
		break;
	}//switch(_type)
}

/******************************************************************/

cMovingAction::cMovingAction(const weak_ptr<cActor> & actor,
							 const weak_ptr<cBehaviorTree> & tree,
							 const ImVec2 & position,
							 weak_ptr<cTask> parent)
	: cTask(tree, "Move", position, parent)
	, _actor(actor)
	, _speed(0.0f)
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
	
	if (_state == eState::RUNNING)
	{
		auto transformPtr = _transform.lock();
		transformPtr->Move(_speed);
	}

	return _state;
}

void cMovingAction::RenderInfo()
{
	auto actorPtr = _actor.lock();
	auto clips = actorPtr->GetAnimator().lock()->GetClips();

	ImGui::TextColored(ActionTextColor, _taskName.c_str());

	ImGui::NewLine();

	ImGui::PushID(0);
	ImGui::Text("Speed");
	ImGui::InputFloat("", &_speed);
	ImGui::PopID();
}

void cMovingAction::LoadJson(Json::Value & root)
{
	LoadDefaultInfo(root);

	Json::GetValue(root, "Speed", _speed);
}

void cMovingAction::SaveJson(Json::Value & root)
{
	Json::Value task;

	SaveDefaultInfo(task);

	Json::SetValue(task, "Speed", _speed);

	root.append(task);
}

/******************************************************************/

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
			case eRotationType::ANGLE:
				RotateAngle();
				break;
		}
	}

	return _state;
}

void cRotatingAction::RenderInfo()
{
	auto actorPtr = _actor.lock();
	auto clips = actorPtr->GetAnimator().lock()->GetClips();

	ImGui::TextColored(ActionTextColor, _taskName.c_str());

	ImGui::NewLine();

	ImGui::PushID(0);
	{
		ImGui::Text("Rotation Type :");

		const char* items[] = { "To Target", "User Input", "Angle" };
		int type = static_cast<int>(_rotationType);
		ImGui::Combo("", &type, items, IM_ARRAYSIZE(items));
		_rotationType = static_cast<eRotationType>(type);
	}
	ImGui::PopID();

	ImGui::PushID(1);
	{
		if (_rotationType == eRotationType::ANGLE)
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
	auto blackboardPtr = actorPtr->GetBlackboard().lock();

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
	auto blackboard = actorPtr->GetBlackboard().lock();
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
	//todo : 캐릭터 위치 가져옴
	//todo : 각도만큼 회전
}

void cRotatingAction::Rotate(OUT shared_ptr<sTransform> transformPtr,
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

/******************************************************************/

cAttackAction::cAttackAction(const weak_ptr<cActor> & actor,
								   const weak_ptr<cBehaviorTree>& tree,
								   const ImVec2& position,
								   weak_ptr<cTask> parent)
	: cTask(tree, "Attack", position, parent)
	, _actor(actor)
	, _selectedClip("")
	, _blendTime(0.2f)
	, _comboFrame(0)
	, _attackNumber(1)
{
}

cAttackAction::~cAttackAction()
{
}

std::unique_ptr<cTask> cAttackAction::Clone() const
{
	auto task = make_unique<cAttackAction>(_actor, _tree, _pos);

	task->_clip = _clip;
	task->_blendTime = _blendTime;
	task->_selectedClip = _selectedClip;
	task->_comboFrame = _comboFrame;

	return move(task);
}

cTask::eState cAttackAction::Run()
{
	auto actorPtr = _actor.lock();
	if (!actorPtr)
		return eState::FAILURE;

	auto animatorPtr = actorPtr->GetAnimator().lock();
	auto blackboardPtr = actorPtr->GetBlackboard().lock();

	const int& attackNumber = blackboardPtr->GetInt("AttackNumber");

	//현재 공격 번호가 본인보다 높으면 성공 반환
	if (attackNumber > _attackNumber)
		return _state = eState::SUCCESS;

	const int& attackState = blackboardPtr->GetInt("AttackState");

	//최초 진입
	if (attackState == 1)
	{
		cDebug::Log("%d", _attackNumber);

		animatorPtr->SetCurrentClip(_clip, false, _blendTime, false);
		_tree.lock()->SetCurrentTask(shared_from_this());
		
		//공격 번호 세팅
		blackboardPtr->SetInt("AttackNumber", _attackNumber);

		//공격방향은 카메라 방향
		D3DXVECTOR3 camDir;
		sGlobalVariable::MainCamera.lock()->
			GetCameraDirection(camDir);
		camDir.y = 0.0f;
		D3DXVec3Normalize(&camDir, &camDir);
		blackboardPtr->SetVector3("TargetDirection", camDir);

		//다음 상태 변환
		blackboardPtr->SetInt("AttackState", 2);
		_state = eState::RUNNING;
	}

	//공격 중
	if (attackState == 2 || attackState == 3)
	{
		//콤보 가능 상태 전환
		if (animatorPtr->GetCurrentFrameCount() >= _comboFrame)
			blackboardPtr->SetInt("AttackState", 3);

		//애니메이션 끝나면 콤보 종료
		if (animatorPtr->IsEndAnimation())
		{
			blackboardPtr->SetInt("AttackState", 0);
			blackboardPtr->SetBool("MeleeAttack", false);
			blackboardPtr->SetInt("AttackNumber", 0);

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

		return _state = eState::SUCCESS;
	}

	return _state = eState::RUNNING;
}

void cAttackAction::RenderInfo()
{
	auto actorPtr = _actor.lock();
	auto clips = actorPtr->GetAnimator().lock()->GetClips();

	ImGui::TextColored(ActionTextColor, _taskName.c_str());

	ImGui::NewLine();

	ImGui::PushID(0);
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

	ImGui::PushID(1);
	ImGui::Text("Blending Time :");
	ImGui::InputFloat("", &_blendTime);
	ImGui::PopID();

	ImGui::PushID(2);
	{
		ImGui::Text("Attack Number");
		ImGui::InputInt("", &_attackNumber);
	}
	ImGui::PopID();

	ImGui::PushID(3);
	{
		ImGui::Text("Combo Frame");
		ImGui::InputInt("", &_comboFrame);
	}
	ImGui::PopID();
}

void cAttackAction::LoadJson(Json::Value & root)
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
	Json::GetValue(root, "Combo Frame", _comboFrame);
	Json::GetValue(root, "Attack Number", _attackNumber);
}

void cAttackAction::SaveJson(Json::Value & root)
{
	Json::Value task;

	SaveDefaultInfo(task);

	string clipName = "";
	auto clipPtr = _clip.lock();
	if (clipPtr)
		clipName = cString::String(clipPtr->GetName());
	Json::SetValue(task, "Clip Name", clipName);

	Json::SetValue(task, "Blend Time", _blendTime);
	Json::SetValue(task, "Combo Frame", _comboFrame);
	Json::SetValue(task, "Attack Number", _attackNumber);

	root.append(task);
}