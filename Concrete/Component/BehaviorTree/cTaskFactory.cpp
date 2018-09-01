#include "stdafx.h"
#include "cTaskFactory.h"
#include "Conditions.h"
#include "Actions.h"

map<cTaskFactory::eActionType, string> cTaskFactory::_actions;
map<cTaskFactory::eConditionType, string> cTaskFactory::_conditions;
bool cTaskFactory::_init = false;

cTaskFactory::cTaskFactory()
{
	if (_init == false)
	{
		InitActions();
		InitConditions();
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
			return make_unique<cSingleAnimation>(actor, tree, position);
		case cTaskFactory::eActionType::MOVING:
			return make_unique<cMovingAction>(actor, tree, position);
	}
	return nullptr;
}

unique_ptr<cTask> cTaskFactory::CreateCondition(cTaskFactory::eConditionType type, weak_ptr<cBlackboard> blackboard, weak_ptr<cBehaviorTree> tree, const ImVec2 & position)
{
	switch (type)
	{
		case cTaskFactory::eConditionType::BOOL:
		{	
			return make_unique<cBoolCondition>(blackboard, tree, position);
		}
		case cTaskFactory::eConditionType::VALUE_COMPARE:
			return make_unique<cValueCompareCondition>(blackboard, tree, position);
	}
	return nullptr;
}

unique_ptr<cTask> cTaskFactory::CreateComposition(weak_ptr<cBehaviorTree> tree, const ImVec2 & position)
{
	return make_unique<cCompositeTask>(tree, position);
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

unique_ptr<cTask> cTaskFactory::ShowConditionMenu(weak_ptr<cBlackboard> blackboard, weak_ptr<cBehaviorTree> tree, const ImVec2 & position)
{
	unique_ptr<cTask> task;
	int i = 0;
	for (auto&& condition : _conditions)
	{
		ImGui::PushID(i++);

		if (ImGui::MenuItem(condition.second.c_str(), nullptr, false))
		{
			task = CreateCondition(condition.first, blackboard, tree, position);
		}

		ImGui::PopID();
	}
	
	return task;
}

void cTaskFactory::InitActions()
{
	_actions[eActionType::ANIMATION] = "Animation";
	_actions[eActionType::MOVING] = "Moving";
}

void cTaskFactory::InitConditions()
{
	_conditions[eConditionType::BOOL] = "Bool";
	_conditions[eConditionType::VALUE_COMPARE] = "Value Compare";
}