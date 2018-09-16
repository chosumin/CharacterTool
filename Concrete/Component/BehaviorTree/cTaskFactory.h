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
		BOOL_COMPARE, VALUE_COMPARE,
		IS_NEAR,
	};

	enum class eActionType
	{
		SET_DATA, ANIMATE_ONE, ANIMATE_MULTI, MOVE, ROTATE,
		COMBO_ATTACK, SINGLE_ATTACK,
		TREE_RUNNER
	};

	enum class eDecoratorType
	{
		INVERTER, SUCCEEDER
	};
public:
	cTaskFactory();
	~cTaskFactory();

	//�׼� ���� ��� ����
	unique_ptr<cTask> CreateAction(eActionType type,
								   const weak_ptr<cActor> & actor,
								   const weak_ptr<cBehaviorTree> & tree,
								   const ImVec2& position);

	//���� ���� ��� ����
	unique_ptr<cTask> CreateCondition(eConditionType type,
									  const weak_ptr<cBlackboard> & blackboard,
									  const weak_ptr<cBehaviorTree> & tree,
									  const ImVec2& position);

	//������Ʈ �½�ũ ����
	unique_ptr<cTask> CreateComposition(weak_ptr<cBehaviorTree> tree,
										const ImVec2& position);

	unique_ptr<cTask> CreateDecorator(eDecoratorType type,
									  weak_ptr<cBehaviorTree> tree,
									  const ImVec2& position);

	//�ش� �½�ũ�� ������ �½�ũ ��ȯ
	shared_ptr<cTask> CopyTask(const weak_ptr<cTask> & copiedTarget);

	void CreateTask(weak_ptr<cTask> parent, Json::Value & root,
					weak_ptr<cActor> actor);

	//�׼� ��� �����ֱ� �� ���� ImGui
	unique_ptr<cTask> ShowActionMenu(weak_ptr<cActor> actor,
									 weak_ptr<cBehaviorTree> tree,
									 const ImVec2& position);

	//���� ��� �����ֱ� �� ���� ImGui
	unique_ptr<cTask> ShowConditionMenu(weak_ptr<cBlackboard> blackboard,
										weak_ptr<cBehaviorTree> tree,
										const ImVec2& position);

	//����� ��� �����ֱ� �� ���� ImGui
	unique_ptr<cTask> ShowDecoratorMenu(weak_ptr<cBehaviorTree> tree,
										const ImVec2& position);
private:
	void InitActions();
	void InitConditions();
	void InitDecorators();

	//�½�ũ ���� ����
	void SetTaskOnCopyEvent(weak_ptr<cTask> task);
private:
	static unordered_map<eConditionType, unique_ptr<cTask>> _conditions;
	static unordered_map<eActionType, unique_ptr<cTask>> _actions;
	static unordered_map<eDecoratorType, unique_ptr<cTask>> _decorators;
	static bool _init;
};