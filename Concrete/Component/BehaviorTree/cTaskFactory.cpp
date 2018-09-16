#include "stdafx.h"
#include "cTaskFactory.h"
#include "Decorators.h"
#include "./Condition/ValueCompares.h"
#include "./Condition/cIsNearCondition.h"
#include "./Action/Animations.h"
#include "./Action/cSingleAttackAction.h"
#include "./Action/cComboAttackAction.h"
#include "./Action/cDataSetter.h"
#include "./Action/cMovingAction.h"
#include "./Action/cRotatingAction.h"
#include "./Action/cBTreeRunner.h"
#include "./GameObject/cActor.h"

unordered_map<cTaskFactory::eActionType, unique_ptr<cTask>> cTaskFactory::_actions;
unordered_map<cTaskFactory::eConditionType, unique_ptr<cTask>> cTaskFactory::_conditions;
unordered_map<cTaskFactory::eDecoratorType, unique_ptr<cTask>> cTaskFactory::_decorators;
bool cTaskFactory::_init = false;

cTaskFactory::cTaskFactory()
{
	if (_init == false)
	{
		InitActions();
		InitConditions();
		InitDecorators();
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
		case cTaskFactory::eActionType::COMBO_ATTACK:
			return make_unique<cComboAttackAction>(actor, tree, position);
		case cTaskFactory::eActionType::SINGLE_ATTACK:
			return make_unique<cSingleAttackAction>(actor, tree, position);
		case cTaskFactory::eActionType::TREE_RUNNER:
			return make_unique<cBTreeRunner>(tree, position);
	}
	return nullptr;
}

unique_ptr<cTask> cTaskFactory::CreateCondition(cTaskFactory::eConditionType type, const weak_ptr<cBlackboard> & blackboard, const weak_ptr<cBehaviorTree> & tree, const ImVec2 & position)
{
	switch (type)
	{
		case cTaskFactory::eConditionType::BOOL_COMPARE:
			return make_unique<cBoolCondition>(blackboard, tree, position);
		case cTaskFactory::eConditionType::VALUE_COMPARE:
			return make_unique<cValueCompareCondition>(blackboard, tree, position);
		case cTaskFactory::eConditionType::IS_NEAR:
			return make_unique<cIsNearCondition>(blackboard, tree, position);
	}
	return nullptr;
}

unique_ptr<cTask> cTaskFactory::CreateComposition(weak_ptr<cBehaviorTree> tree, const ImVec2 & position)
{
	return make_unique<cCompositeTask>(tree, position);
}

unique_ptr<cTask> cTaskFactory::CreateDecorator(
	eDecoratorType type,
	weak_ptr<cBehaviorTree> tree,
	const ImVec2& position)
{
	switch (type)
	{
		case eDecoratorType::INVERTER:
			return make_unique<cInverter>(tree, position);
		case eDecoratorType::SUCCEEDER:
			return make_unique<cSucceeder>(tree, position);
	}
	return nullptr;
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

		//조건 태스크 생성
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

		//액션 태스크 생성
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

		//장식자 태스크 생성
		if (!create)
		{
			for (auto&& decorator : _decorators)
			{
				if (decorator.second->GetName() == taskName)
				{
					task = CreateDecorator(decorator.first, bTree, ImVec2{ 0,0 });
					create = true;
				}
			}
		}

		//복합 태스크 생성
		if (!create)
			task = CreateComposition(bTree, ImVec2{ 0,0 });

		//태스크 생성 완료시
		if (task)
		{
			//json 파싱
			task->LoadJson(taskValue);

			//부모 연결
			task->SetParent(parent);

			//자식 재귀
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
		//자식 재귀
		SetTaskOnCopyEvent(child);

		//부모 저장
		child->SetParent(taskPtr);

		//상대 위치 저장
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

unique_ptr<cTask> cTaskFactory::ShowDecoratorMenu(weak_ptr<cBehaviorTree> tree, const ImVec2 & position)
{
	unique_ptr<cTask> task;
	int i = 0;
	for (auto&& decorator : _decorators)
	{
		ImGui::PushID(i++);

		if (ImGui::MenuItem(decorator.second->GetName().c_str(), nullptr, false))
		{
			task = CreateDecorator(decorator.first, tree, position);
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

	_actions[eActionType::COMBO_ATTACK] = make_unique<cComboAttackAction>(weak_ptr<cActor>(), weak_ptr<cBehaviorTree>(), ImVec2{ 0,0 });

	_actions[eActionType::SINGLE_ATTACK] = make_unique<cSingleAttackAction>(weak_ptr<cActor>(), weak_ptr<cBehaviorTree>(), ImVec2{ 0,0 });

	_actions[eActionType::TREE_RUNNER] = make_unique<cBTreeRunner>(weak_ptr<cBehaviorTree>(), ImVec2{ 0,0 });
}

void cTaskFactory::InitConditions()
{
	_conditions[eConditionType::BOOL_COMPARE] = make_unique<cBoolCondition>(weak_ptr<cBlackboard>(),weak_ptr<cBehaviorTree>(), ImVec2{ 0,0 });
	_conditions[eConditionType::VALUE_COMPARE] = make_unique<cValueCompareCondition>(weak_ptr<cBlackboard>(),weak_ptr<cBehaviorTree>(), ImVec2{ 0,0 });
	_conditions[eConditionType::IS_NEAR] = make_unique<cIsNearCondition>(weak_ptr<cBlackboard>(), weak_ptr<cBehaviorTree>(), ImVec2{ 0,0 });
}

void cTaskFactory::InitDecorators()
{
	_decorators[eDecoratorType::INVERTER] = make_unique<cInverter>(weak_ptr<cBehaviorTree>(), ImVec2{ 0,0 });
	_decorators[eDecoratorType::SUCCEEDER] = make_unique<cSucceeder>(weak_ptr<cBehaviorTree>(), ImVec2{ 0,0 });
}
