#include "stdafx.h"
#include "Conditions.h"
#include "./Command/cInputHandler.h"
#include "./GameObject/cActor.h"
#include "./Interface/iAction.h"
#include "./Blackboard/cBlackboard.h"
#include "./Helper/cMath.h"

cBoolCondition::cBoolCondition(weak_ptr<cBlackboard> blackboard,
							   weak_ptr<cBehaviorTree> tree,
							   const ImVec2 & position,
							   const weak_ptr<cTask>& parent)
	: cTask(tree, "Bool Condition", position, parent)
	, _blackboard(blackboard)
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

	return move(clone);
}

cTask::eState cBoolCondition::Run()
{
	auto blackboardPtr = _blackboard.lock();
	if (blackboardPtr)
	{
		return blackboardPtr->GetBool(_boolParam) == _boolean ?
			eState::SUCCESS : eState::FAILURE;
	}

	return eState::NONE;
}

void cBoolCondition::RenderInfo()
{
	ImGui::TextColored(ImVec4(0, 255, 0, 1), _taskName.c_str());

	ImGui::NewLine();

	ImGui::PushID(0);
	if (ImGui::BeginCombo("", _boolParam.c_str()))
	{
		auto blackboardPtr = _blackboard.lock();
		if (blackboardPtr)
		{
			for (const auto& key : blackboardPtr->GetBools())
			{
				const string& name = key.first;
				bool isSelected = _boolParam == name;
				if (ImGui::Selectable(name.c_str(), isSelected))
					_boolParam = name;

				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}
	ImGui::PopID();

	ImGui::Text("Is");

	ImGui::PushID(1);
	{
		const char* items[] = { "True", "False" };

		int type = _boolean ? 0 : 1;
		
		ImGui::Combo("", &type, items, IM_ARRAYSIZE(items));
		_boolean = type == 0 ? true : false;
	}
	ImGui::PopID();
}

/*******************************************************************/

cValueCompareCondition::cValueCompareCondition(weak_ptr<cBlackboard> blackboard, weak_ptr<cBehaviorTree> tree, const ImVec2 & position, const weak_ptr<cTask>& parent)
	: cTask(tree, "Value Compare", position, parent)
	, _blackboard(blackboard)
	, _compare(0, 0, 0)
{
}

cValueCompareCondition::~cValueCompareCondition()
{
}

std::unique_ptr<cTask> cValueCompareCondition::Clone() const
{
	auto clone = make_unique<cValueCompareCondition>(_blackboard, _tree, _pos, _parent);

	clone->_valueParam = _valueParam;
	clone->_compare = _compare;

	return move(clone);
}

cTask::eState cValueCompareCondition::Run()
{
	bool flag = false;
	auto blackboardPtr = _blackboard.lock();
	if (blackboardPtr)
	{
		D3DXVECTOR3 direction;
		blackboardPtr->GetDirection(direction);

		if (!cMath::IsVec3Equal(direction, _compare))
			return eState::SUCCESS;
	}

	return eState::FAILURE;
}

void cValueCompareCondition::RenderInfo()
{
	ImGui::TextColored(ImVec4(0, 255, 0, 1), _taskName.c_str());

	ImGui::NewLine();

	ImGui::PushID(0);
	{
		auto blackboardPtr = _blackboard.lock();
		if (blackboardPtr)
		{
			D3DXVECTOR3 direction;
			blackboardPtr->GetDirection(direction);
		}
	}
	ImGui::PopID();

	ImGui::Text("Is Not Equal To");

	ImGui::PushID(1);
	{
		ImGui::InputFloat3("", _compare);
	}
	ImGui::PopID();
}
