#pragma once
#include "./BehaviorTree/BaseTasks.h"

class cActor;
class cKeyCondition;
class cBlackboard;
class cTaskFactory
{
public:
	enum class eConditionType
	{
		BOOL, VALUE_COMPARE
	};

	enum class eActionType
	{
		ANIMATION, MOVING
	};
public:
	cTaskFactory();
	~cTaskFactory();

	//�׼� ���� ��� ����
	unique_ptr<cTask> CreateAction(cTaskFactory::eActionType type,
								   weak_ptr<cActor> actor,
								   weak_ptr<cBehaviorTree> tree,
								   const ImVec2& position);

	//���� ���� ��� ����
	unique_ptr<cTask> CreateCondition(cTaskFactory::eConditionType type,
									  weak_ptr<cBlackboard> blackboard,
									  weak_ptr<cBehaviorTree> tree,
									  const ImVec2& position);

	//������Ʈ �½�ũ ����
	unique_ptr<cTask> CreateComposition(weak_ptr<cBehaviorTree> tree,
										const ImVec2& position);

	//�׼� ��� �����ֱ� �� ���� ImGui
	unique_ptr<cTask> ShowActionMenu(weak_ptr<cActor> actor,
									 weak_ptr<cBehaviorTree> tree,
									 const ImVec2& position);

	//���� ��� �����ֱ� �� ���� ImGui
	unique_ptr<cTask> ShowConditionMenu(weak_ptr<cBlackboard> blackboard,
										weak_ptr<cBehaviorTree> tree,
										const ImVec2& position);
private:
	void InitActions();
	void InitConditions();
private:
	static map<eConditionType, string> _conditions;
	static map<eActionType, string> _actions;
	static bool _init;
};