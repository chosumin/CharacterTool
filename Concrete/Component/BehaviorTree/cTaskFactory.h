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
		BOOL_COMPARE, VALUE_COMPARE
	};

	enum class eActionType
	{
		SET_DATA, ANIMATE_ONE, ANIMATE_MULTI, MOVE, ROTATE,
		ATTACK,
	};
public:
	cTaskFactory();
	~cTaskFactory();

	//액션 리프 노드 생성
	unique_ptr<cTask> CreateAction(cTaskFactory::eActionType type,
								   const weak_ptr<cActor> & actor,
								   const weak_ptr<cBehaviorTree> & tree,
								   const ImVec2& position);

	//조건 리프 노드 생성
	unique_ptr<cTask> CreateCondition(cTaskFactory::eConditionType type,
									  const weak_ptr<cBlackboard> & blackboard,
									  const weak_ptr<cBehaviorTree> & tree,
									  const ImVec2& position);

	//컴포지트 태스크 생성
	unique_ptr<cTask> CreateComposition(weak_ptr<cBehaviorTree> tree,
										const ImVec2& position);

	//해당 태스크를 복사한 태스크 반환
	shared_ptr<cTask> CopyTask(const weak_ptr<cTask> & copiedTarget);

	void CreateTask(weak_ptr<cTask> parent, Json::Value & root,
					weak_ptr<cActor> actor);

	//액션 목록 보여주기 및 생성 ImGui
	unique_ptr<cTask> ShowActionMenu(weak_ptr<cActor> actor,
									 weak_ptr<cBehaviorTree> tree,
									 const ImVec2& position);

	//조건 목록 보여주기 및 생성 ImGui
	unique_ptr<cTask> ShowConditionMenu(weak_ptr<cBlackboard> blackboard,
										weak_ptr<cBehaviorTree> tree,
										const ImVec2& position);
private:
	void InitActions();
	void InitConditions();

	//태스크 정보 갱신
	void SetTaskOnCopyEvent(weak_ptr<cTask> task);
private:
	static map<eConditionType, unique_ptr<cTask>> _conditions;
	static map<eActionType, unique_ptr<cTask>> _actions;
	static bool _init;
};