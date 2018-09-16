#pragma once
#include "./BehaviorTree/BaseTasks.h"

class cActor;
class cBlackboard;
class cIsNearCondition : public cTask
{
public:
	cIsNearCondition(weak_ptr<cBlackboard> blackboard,
				   weak_ptr<cBehaviorTree> tree,
				   const ImVec2& position,
				   const weak_ptr<cTask>& parent = weak_ptr<cTask>());
	~cIsNearCondition();

	// cTask��(��) ���� ��ӵ�
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual eState Run() override;
	virtual void RenderInfo() override;

	// iFile ���
	virtual void LoadJson(Json::Value& root) override;
	virtual void SaveJson(Json::Value& root) override;
private:
	weak_ptr<cBlackboard> _blackboard;

	float _searchRange;
};
