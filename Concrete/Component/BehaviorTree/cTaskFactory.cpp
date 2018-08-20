#include "stdafx.h"
#include "cTaskFactory.h"
#include "cKeyCondition.h"
#include "Actions.h"

map<eKeyType, pair<DWORD, string>> cTaskFactory::_keyList;
map<cTaskFactory::eActionType, string> cTaskFactory::_actions;
map<cTaskFactory::eConditionType, string> cTaskFactory::_conditions;
bool cTaskFactory::_init = false;

cTaskFactory::cTaskFactory()
{
	if (_init == false)
	{
		InitActions();
		InitConditions();
		InitKeyList();
		_init = true;
	}
}

cTaskFactory::~cTaskFactory()
{
}

unique_ptr<cTask> cTaskFactory::CreateAction(cTaskFactory::eActionType type, weak_ptr<cActor> actor, weak_ptr<cBehaviorTree> tree, const ImVec2 & position)
{
	switch (type)
	{
		case cTaskFactory::eActionType::ANIMATION:
			return make_unique<cAnimationOnly>(actor, tree, position);
	}
	return nullptr;
}

unique_ptr<cTask> cTaskFactory::CreateCondition(cTaskFactory::eConditionType type, weak_ptr<cBehaviorTree> tree, const ImVec2 & position)
{
	switch (type)
	{
		case cTaskFactory::eConditionType::KEY:
			return make_unique<cKeyCondition>(tree, position);
	}
	return nullptr;
}

unique_ptr<cTask> cTaskFactory::CreateSelector(const ImVec2 & position)
{
	return make_unique<cSelector>(position);
}


unique_ptr<cTask> cTaskFactory::CreateSequence(const ImVec2 & position)
{
	return make_unique<cSequence>(position);
}

unique_ptr<cTask> cTaskFactory::ShowActionMenu(weak_ptr<cActor> actor, weak_ptr<cBehaviorTree> tree, const ImVec2 & position)
{
	unique_ptr<cTask> task;
	int i = 0;
	for (auto&& action : _actions)
	{
		ImGui::PushID(i++);

		if (ImGui::MenuItem(action.second.c_str(), nullptr, false))
		{
			task = CreateAction(action.first, actor, tree, position);
		}

		ImGui::PopID();
	}

	return task;
}

unique_ptr<cTask> cTaskFactory::ShowConditionMenu(weak_ptr<cBehaviorTree> tree, const ImVec2 & position)
{
	unique_ptr<cTask> task;
	int i = 0;
	for (auto&& condition : _conditions)
	{
		ImGui::PushID(i++);

		if (ImGui::MenuItem(condition.second.c_str(), nullptr, false))
		{
			task = CreateCondition(condition.first, tree, position);
		}

		ImGui::PopID();
	}
	
	return task;
}

const map<eKeyType, pair<DWORD, string>> & cTaskFactory::GetKeyList()
{
	return _keyList;
}

void cTaskFactory::InitActions()
{
	_actions[eActionType::ANIMATION] = "Animation";
}

void cTaskFactory::InitConditions()
{
	_conditions[eConditionType::KEY] = "Key Condition";
}

void cTaskFactory::InitKeyList()
{
	_keyList[eKeyType::MoveForward] = pair<DWORD, string>('W', "MoveFoward");

	_keyList[eKeyType::MoveBackward] = pair<DWORD, string>('S', "MoveBackward");

	_keyList[eKeyType::MoveLeft] = pair<DWORD, string>('A', "MoveLeft");

	_keyList[eKeyType::MoveRight] = pair<DWORD, string>('D', "MoveRight");

	_keyList[eKeyType::MeleeAttack] = pair<DWORD, string>(0, "MeleeAttack");
}
