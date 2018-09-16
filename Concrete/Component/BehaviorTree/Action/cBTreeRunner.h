#pragma once
#include "./BehaviorTree/BaseTasks.h"

class cActor;
class cBTreeRunner : public cTask
{
public:
	cBTreeRunner(const weak_ptr<cBehaviorTree> & tree,
				 const ImVec2& position,
				 weak_ptr<cTask> parent = weak_ptr<cTask>());
	~cBTreeRunner();

	// cTask��(��) ���� ��ӵ�
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual eState Run() override;
	virtual void RenderInfo() override;

	// iFile ���
	virtual void LoadJson(Json::Value& root) override;
	virtual void SaveJson(Json::Value& root) override;
};
