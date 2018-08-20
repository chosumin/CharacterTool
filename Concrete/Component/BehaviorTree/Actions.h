#pragma once
#include "./BehaviorTree/BaseTasks.h"

class cActor;

//class cAttackAction : public cTask
//{
//public:
//	cAttackAction(weak_ptr<cActor> actor);
//	~cAttackAction();
//
//	// cTask��(��) ���� ��ӵ�
//	virtual bool Run() override;
//private:
//	weak_ptr<cActor> _actor;
//};

class cAnimationOnly : public cTask
{
public:
	cAnimationOnly(weak_ptr<cActor> actor,
				   weak_ptr<cBehaviorTree> tree,
				   const ImVec2& position);
	~cAnimationOnly();

	// cTask��(��) ���� ��ӵ�
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual bool Run() override;
	virtual void Update() override;
	virtual void RenderInfo() override;
private:
	weak_ptr<cActor> _actor;
	weak_ptr<class cAnimClip> _clip;
	bool _loop;
	string _selectedClip;
};