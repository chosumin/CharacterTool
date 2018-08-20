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

	//액션 리프 노드 생성
	unique_ptr<cTask> CreateAction(cTaskFactory::eActionType type,
								   weak_ptr<cActor> actor,
								   weak_ptr<cBehaviorTree> tree,
								   const ImVec2& position);

	//조건 리프 노드 생성
	unique_ptr<cTask> CreateCondition(cTaskFactory::eConditionType type,
									  weak_ptr<cBehaviorTree> tree,
									  const ImVec2& position);

	//셀렉터 생성
	unique_ptr<cTask> CreateSelector(const ImVec2& position);

	//시퀀스 생성
	unique_ptr<cTask> CreateSequence(const ImVec2& position);

	//액션 목록 보여주기 및 생성 ImGui
	unique_ptr<cTask> ShowActionMenu(weak_ptr<cActor> actor,
									 weak_ptr<cBehaviorTree> tree,
									 const ImVec2& position);

	//조건 목록 보여주기 및 생성 ImGui
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