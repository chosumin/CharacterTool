#pragma once
#include "./BehaviorTree/BaseTasks.h"

class cActor;

class cAttackAction : public cTask
{
public:
	cAttackAction(weak_ptr<cActor> actor);
	~cAttackAction();

	// cTask��(��) ���� ��ӵ�
	virtual bool Run() override;
private:
	weak_ptr<cActor> _actor;
};