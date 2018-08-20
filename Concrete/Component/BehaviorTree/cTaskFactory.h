#pragma once
#include "./BehaviorTree/BaseTasks.h"

enum class eKeyType;
class cActor;
class cKeyCondition;
class cTaskFactory
{
public:
	static const map<eKeyType, pair<DWORD, string>>& GetKeyList();

	enum class eConditionType
	{
		KEY,
	};

	enum class eActionType
	{
		ANIMATION, 
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
									  weak_ptr<cBehaviorTree> tree,
									  const ImVec2& position);

	//������ ����
	unique_ptr<cTask> CreateSelector(const ImVec2& position);

	//������ ����
	unique_ptr<cTask> CreateSequence(const ImVec2& position);

	//�׼� ��� �����ֱ� �� ���� ImGui
	unique_ptr<cTask> ShowActionMenu(weak_ptr<cActor> actor,
									 weak_ptr<cBehaviorTree> tree,
									 const ImVec2& position);

	//���� ��� �����ֱ� �� ���� ImGui
	unique_ptr<cTask> ShowConditionMenu(weak_ptr<cBehaviorTree> tree,
										const ImVec2& position);
private:
	void InitActions();
	void InitConditions();
	void InitKeyList();
private:
	static map<eKeyType, pair<DWORD, string>> _keyList;
	static map<eConditionType, string> _conditions;
	static map<eActionType, string> _actions;
	static bool _init;
};