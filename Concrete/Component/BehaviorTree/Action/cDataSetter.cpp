#include "stdafx.h"
#include "cDataSetter.h"
#include "./GameObject/cActor.h"
#include "./Blackboard/cBlackboard.h"

cDataSetter::cDataSetter(const weak_ptr<cActor>& actor,
						 const weak_ptr<cBehaviorTree>& tree,
						 const ImVec2 & position,
						 weak_ptr<cTask> parent)
	: cTask(tree, "Set Blackboard Data", position, parent)
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
