#include "stdafx.h"
#include "cTaskFactory.h"
#include "Conditions.h"
#include "Animations.h"
#include "Actions.h"
#include "./GameObject/cActor.h"

map<cTaskFactory::eActionType, unique_ptr<cTask>> cTaskFactory::_actions;
map<cTaskFactory::eConditionType, unique_ptr<cTask>> cTaskFactory::_conditions;
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

unique_ptr<cTask> cTaskFactory::CreateAction(cTaskFactory::eActionType type, const weak_ptr<cActor> & actor, const weak_ptr<cBehaviorTree> & tree, const ImVec2 & position)
{
	switch (type)
	{
		case cTaskFactory::eActionType::SET_DATA:
			return make_unique<cDataSetter>(actor, tree, position);
		case cTaskFactory::eActionType::ANIMATE_ONE:
			return make_unique<cSingleAnimation>(actor, tree, position);
		case cTaskFactory::eActionType::ANIMATE_MULTI:
			return make_unique<cMultiAnimation>(actor, tree, position);
		case cTaskFactory::eActionType::MOVE:
			return make_unique<cMovingAction>(actor, tree, position);
		case cTaskFactory::eActionType::ROTATE:
			return make_unique<cRotatingAction>(actor, tree, position);
		case cTaskFactory::eActionType::ATTACK:
			return make_unique<cAttackAction>(actor, tree, position);
	}
	return nullptr;
}

unique_ptr<cTask> cTaskFactory::CreateCondition(cTaskFactory::eConditionType type, const weak_ptr<cBlackboard> & blackboard, const weak_ptr<cBehaviorTree> & tree, const ImVec2 & position)
{
	switch (type)
	{
		case cTaskFactory::eConditionType::BOOL_COMPARE:
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

shared_ptr<cTask> cTaskFactory::CopyTask(const weak_ptr<cTask> & copiedTarget)
{
	auto targetPtr = copiedTarget.lock();
	if (!targetPtr)
		return nullptr;

	shared_ptr<cTask> clone = targetPtr->Clone();
	
	SetTaskOnCopyEvent(clone);

	return clone;
}

void cTaskFactory::CreateTask(weak_ptr<cTask> parent,
							  Json::Value & root,
							  weak_ptr<cActor> actor)
{
	auto actorPtr = actor.lock();
	auto bTree = actorPtr->GetBehaviorTree();
	auto blackboard = actorPtr->GetBlackboard();

	for (auto&& taskValue : root)
	{
		shared_ptr<cTask> task;

		string taskName;
		Json::GetValue(taskValue, "Task Name", taskName);

		bool create = false;

		//���� �½�ũ ����
		if (!create)
		{
			for (auto&& condition : _conditions)
			{
				if (condition.second->GetName() == taskName)
				{
					task = CreateCondition(condition.first, blackboard, bTree, ImVec2{ 0,0 });
					create = true;
				}
			}
		}

		//�׼� �½�ũ ����
		if (!create)
		{
			for (auto&& action : _actions)
			{
				if (action.second->GetName() == taskName)
				{
					task = CreateAction(action.first, actor, bTree, ImVec2{ 0,0 });
					create = true;
				}
			}
		}

		//���� �½�ũ ����
		if (!create)
			task = CreateComposition(bTree, ImVec2{ 0,0 });

		//�½�ũ ���� �Ϸ��
		if (task)
		{
			//json �Ľ�
			task->LoadJson(taskValue);

			//�θ� ����
			task->SetParent(parent);

			//�ڽ� ���
			if (task->GetChildren())
				CreateTask(task, taskValue["Children"], actor);

			parent.lock()->AddChild(task);
		}
	}//for(root)
}

void cTaskFactory::SetTaskOnCopyEvent(weak_ptr<cTask> task)
{
	auto taskPtr = task.lock();

	if (!taskPtr->GetChildren())
		return;

	for (auto&& child : *taskPtr->GetChildren())
	{
		//�ڽ� ���
		SetTaskOnCopyEvent(child);

		//�θ� ����
		child->SetParent(taskPtr);

		//��� ��ġ ����
		ImVec2 parentPos = taskPtr->GetPosition();
		ImVec2 childPos = child->GetPosition();
		child->SetPosition(
			ImVec2{ childPos.x - parentPos.x,
			childPos.y - parentPos.y });
	}
}

unique_ptr<cTask> cTaskFactory::ShowActionMenu(weak_ptr<cActor> actor, weak_ptr<cBehaviorTree> tree, const ImVec2 & position)
{
	unique_ptr<cTask> task;
	int i = 0;
	for (auto&& action : _actions)
	{
		ImGui::PushID(i++);

		if (ImGui::MenuItem(action.second->GetName().c_str(), nullptr, false))
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

		if (ImGui::MenuItem(condition.second->GetName().c_str(), nullptr, false))
		{
			task = CreateCondition(condition.first, blackboard, tree, position);
		}

		ImGui::PopID();
	}
	
	return task;
}

void cTaskFactory::InitActions()
{
	_actions[eActionType::SET_DATA] = make_unique<cDataSetter>(weak_ptr<cActor>(), weak_ptr<cBehaviorTree>(), ImVec2{ 0,0 });

	_actions[eActionType::ANIMATE_ONE] = make_unique<cSingleAnimation>(weak_ptr<cActor>(), weak_ptr<cBehaviorTree>(), ImVec2{ 0,0 });

	_actions[eActionType::ANIMATE_MULTI] = make_unique<cMultiAnimation>(weak_ptr<cActor>(), weak_ptr<cBehaviorTree>(), ImVec2{ 0,0 });

	_actions[eActionType::MOVE] = make_unique<cMovingAction>(weak_ptr<cActor>(), weak_ptr<cBehaviorTree>(), ImVec2{ 0,0 });

	_actions[eActionType::ROTATE] = make_unique<cRotatingAction>(weak_ptr<cActor>(), weak_ptr<cBehaviorTree>(), ImVec2{ 0,0 });

	_actions[eActionType::ATTACK] = make_unique<cAttackAction>(weak_ptr<cActor>(), weak_ptr<cBehaviorTree>(), ImVec2{ 0,0 });
}

void cTaskFactory::InitConditions()
{
	_conditions[eConditionType::BOOL_COMPARE] = make_unique<cBoolCondition>(weak_ptr<cBlackboard>(),weak_ptr<cBehaviorTree>(), ImVec2{ 0,0 });
	_conditions[eConditionType::VALUE_COMPARE] = make_unique<cValueCompareCondition>(weak_ptr<cBlackboard>(),weak_ptr<cBehaviorTree>(), ImVec2{ 0,0 });
}