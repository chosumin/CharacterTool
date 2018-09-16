#include "stdafx.h"
#include "ValueCompares.h"
#include "./Command/cInputHandler.h"
#include "./GameObject/cActor.h"
#include "./Blackboard/cBlackboard.h"
#include "./Helper/cMath.h"

cBoolCondition::cBoolCondition(weak_ptr<cBlackboard> blackboard,
							   weak_ptr<cBehaviorTree> tree,
							   const ImVec2 & position,
							   const weak_ptr<cTask>& parent)
	: cTask(tree, "Bool Condition", position, parent)
	, _blackboard(blackboard)
	, _boolParam("")
	, _boolean(false)
	, _compareType(0)
	, _compareParam("")
	, _isSame(true)
{
}

cBoolCondition::~cBoolCondition()
{
}

std::unique_ptr<cTask> cBoolCondition::Clone() const
{
	auto clone = make_unique<cBoolCondition>(_blackboard, _tree, _pos, _parent);

	clone->_boolParam = _boolParam;
	clone->_boolean = _boolean;
	clone->_compareType = _compareType;
	clone->_compareParam = _compareParam;
	clone->_isSame = _isSame;

	return move(clone);
}

cTask::eState cBoolCondition::Run()
{
	auto blackboardPtr = _blackboard.lock();
	if (blackboardPtr)
	{
		if (_compareType == 0) //유저 입력
		{
			return (blackboardPtr->GetBool(_boolParam) == _boolean) == _isSame ?
				eState::SUCCESS : eState::FAILURE;
		}
		else
		{
			return (blackboardPtr->GetBool(_boolParam) == blackboardPtr->GetBool(_compareParam)) == _isSame ?
				eState::SUCCESS : eState::FAILURE;
		}
	}

	return eState::NONE;
}

void cBoolCondition::RenderInfo()
{
	ImGui::TextColored(ImVec4(0, 255, 0, 1), _taskName.c_str());

	ImGui::NewLine();

	ImGui::PushID(0);
	{
		ImGui::Text("Blackboard Data :");
		ComboBox(_boolParam);
	}
	ImGui::PopID();

	ImGui::PushID(1);
	{
		if (_isSame)
			ImGui::Checkbox("==", &_isSame);
		else
			ImGui::Checkbox("!=", &_isSame);
	}
	ImGui::PopID();

	ImGui::PushID(1);
	{
		const char* items[] = { "User Input :", "Blackboard Data :" };
		ImGui::Combo("", &_compareType, items, IM_ARRAYSIZE(items));
	}
	ImGui::PopID();

	ImGui::PushID(2);
	{
		if (_compareType == 0) //유저 입력
		{
			const char* items[] = { "True", "False" };

			int type = _boolean ? 0 : 1;

			ImGui::Combo("", &type, items, IM_ARRAYSIZE(items));
			_boolean = type == 0 ? true : false;
		}
		else if (_compareType == 1) //블랙보드 데이터
			ComboBox(_compareParam);
	}
	ImGui::PopID();
}

void cBoolCondition::LoadJson(Json::Value & root)
{
	LoadDefaultInfo(root);

	Json::GetValue(root, "Bool Name", _boolParam);
	Json::GetValue(root, "Boolean", _boolean);
	Json::GetValue(root, "Compare Type", _compareType);
	Json::GetValue(root, "Compare Name", _compareParam);
	Json::GetValue(root, "Is Same", _isSame);
}

void cBoolCondition::SaveJson(Json::Value & root)
{
	Json::Value task;

	SaveDefaultInfo(task);

	Json::SetValue(task, "Bool Name", _boolParam);
	Json::SetValue(task, "Boolean", _boolean);
	Json::SetValue(task, "Compare Type", _compareType);
	Json::SetValue(task, "Compare Name", _compareParam);
	Json::SetValue(task, "Is Same", _isSame);

	root.append(task);
}

void cBoolCondition::ComboBox(string & data)
{
	if (ImGui::BeginCombo("", data.c_str()))
	{
		auto blackboardPtr = _blackboard.lock();
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

/*******************************************************************/

cValueCompareCondition::cValueCompareCondition(weak_ptr<cBlackboard> blackboard, weak_ptr<cBehaviorTree> tree, const ImVec2 & position, const weak_ptr<cTask>& parent)
	: cTask(tree, "Value Compare", position, parent)
	, _blackboard(blackboard)
	, _valueParam("")
	, _compareVec3(0, 0, 0)
	, _compareFloat(0.0f)
	, _compareInt(0)
	, _type(0)
	, _inequalitySign(0)
	, _compareType(0)
	, _compareParam("")
{
}

cValueCompareCondition::~cValueCompareCondition()
{
}

std::unique_ptr<cTask> cValueCompareCondition::Clone() const
{
	auto clone = make_unique<cValueCompareCondition>(_blackboard, _tree, _pos, _parent);

	clone->_valueParam = _valueParam;
	clone->_type = _type;
	clone->_compareVec3 = _compareVec3;
	clone->_compareFloat = _compareFloat;
	clone->_compareInt = _compareInt;
	clone->_inequalitySign = _inequalitySign;
	clone->_compareType = _compareType;
	clone->_compareParam = _compareParam;

	return move(clone);
}

cTask::eState cValueCompareCondition::Run()
{
	bool flag = false;
	auto blackboardPtr = _blackboard.lock();

	bool result = false;
	if (blackboardPtr)
	{
		if (_compareType == 1) //블랙보드 데이터와 비교
		{
			if (_type == 0) //float
				_compareFloat = blackboardPtr->GetFloat(_compareParam);
			else if (_type == 1)
				_compareVec3 = blackboardPtr->GetVector3(_compareParam);
			else if (_type == 2)
				_compareInt = blackboardPtr->GetInt(_compareParam);
		}
		
		switch (_type)
		{
			case 0: //float
			{
				float val = blackboardPtr->GetFloat(_valueParam);
				switch (_inequalitySign)
				{
					case 0:
						result = val > _compareFloat;
						break;
					case 1:
						result = val >= _compareFloat;
						break;
					case 2:
						result = val == _compareFloat;
						break;
					case 3:
						result = val != _compareFloat;
						break;
					case 4:
						result = val <= _compareFloat;
						break;
					case 5:
						result = val < _compareFloat;
						break;
				}
			}
			break; //case(float)
			case 1: //vector3
			{
				auto& val = blackboardPtr->GetVector3(_valueParam);
				switch (_inequalitySign)
				{
					case 0:
					case 1:
					case 2:
						result = cMath::IsVec3Equal(val, _compareVec3);
						break;
					case 3:
					case 4:
					case 5:
						result = !cMath::IsVec3Equal(val, _compareVec3);
						break;
				}
			}
			break; //case(vector3)
			case 2: //int
			{
				int val = blackboardPtr->GetInt(_valueParam);
				switch (_inequalitySign)
				{
					case 0:
						result = val > _compareInt;
						break;
					case 1:
						result = val >= _compareInt;
						break;
					case 2:
						result = val == _compareInt;
						break;
					case 3:
						result = val != _compareInt;
						break;
					case 4:
						result = val <= _compareInt;
						break;
					case 5:
						result = val < _compareInt;
						break;
				}
			}
			break; //case(int)
		}//switch(_type)
	}//if(blackboardPtr)

	return _state = result ? eState::SUCCESS : eState::FAILURE;
}

void cValueCompareCondition::RenderInfo()
{
	ImGui::TextColored(ImVec4(0, 255, 0, 1), _taskName.c_str());

	ImGui::NewLine();

	ImGui::PushID(0);
	{
		ImGui::Text("Data Type");
		ImGui::Combo("", &_type, "Float\0Vector3\0Int\0");
	}
	ImGui::PopID();

	ImGui::NewLine();

	ImGui::PushID(1);
	{
		ImGui::Text("Blackboard Data :");
		ComboBox(_valueParam);
	}
	ImGui::PopID();

	ImGui::PushID(2);
	{
		const char* items[] = { ">", ">=", "==", "!=", "<=", "<" };
		ImGui::Combo("", &_inequalitySign, items, IM_ARRAYSIZE(items));
	}
	ImGui::PopID();
	
	ImGui::PushID(3);
	{
		const char* items[] = { "User Input :", "Blackboard Data :" };
		ImGui::Combo("", &_compareType, items, IM_ARRAYSIZE(items));
	}
	ImGui::PopID();

	ImGui::PushID(4);
	{
		if (_compareType == 0) //유저 입력
		{
			switch (_type)
			{
				case 0:
					ImGui::InputFloat("", &_compareFloat);
					break;
				case 1:
					ImGui::InputFloat3("", _compareVec3);
					break;
				case 2:
					ImGui::InputInt("", &_compareInt);
					break;
			}
		}
		else //블랙보드 데이터
		{
			ComboBox(_compareParam);
		}
	}
	ImGui::PopID();
}

void cValueCompareCondition::LoadJson(Json::Value & root)
{
	LoadDefaultInfo(root);

	Json::GetValue(root, "Value Name", _valueParam);
	Json::GetValue(root, "Data Type", _type);
	Json::GetValue(root, "Compare Vector3", _compareVec3);
	Json::GetValue(root, "Compare Float", _compareFloat);
	Json::GetValue(root, "Compare Int", _compareInt);
	Json::GetValue(root, "Inequality Sign", _inequalitySign);
	Json::GetValue(root, "Compare Type", _compareType);
	Json::GetValue(root, "Compare Name", _compareParam);
}

void cValueCompareCondition::SaveJson(Json::Value & root)
{
	Json::Value task;

	SaveDefaultInfo(task);

	Json::SetValue(task, "Value Name", _valueParam);
	Json::SetValue(task, "Data Type", _type);
	Json::SetValue(task, "Compare Vector3", _compareVec3);
	Json::SetValue(task, "Compare Float", _compareFloat);
	Json::SetValue(task, "Compare Int", _compareInt);
	Json::SetValue(task, "Inequality Sign", _inequalitySign);
	Json::SetValue(task, "Compare Type", _compareType);
	Json::SetValue(task, "Compare Name", _compareParam);

	root.append(task);
}

void cValueCompareCondition::ComboBox(string & data)
{
	auto blackboardPtr = _blackboard.lock();
	switch (_type)
	{
		case 0: //float
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
		case 1: //vector3
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
		case 2: //int
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
