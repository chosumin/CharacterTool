#pragma once
#include "./BehaviorTree/BaseTasks.h"

class cActor;
class cSingleAnimation : public cTask
{
public:
	cSingleAnimation(weak_ptr<cActor> actor,
					 weak_ptr<cBehaviorTree> tree,
					 const ImVec2& position,
					 weak_ptr<cTask> parent = weak_ptr<cTask>());
	~cSingleAnimation();

	// cTask을(를) 통해 상속됨
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual eState Run() override;
	virtual void RenderInfo() override;
private:
	weak_ptr<cActor> _actor;
	weak_ptr<class cAnimClip> _clip;
	bool _overwrite;
	string _selectedClip;
};

/******************************************************************/

class cMovingAction : public cTask
{
public:
	cMovingAction(weak_ptr<cActor> actor,
				  weak_ptr<cBehaviorTree> tree,
				  const ImVec2& position,
				  weak_ptr<cTask> parent = weak_ptr<cTask>());
	~cMovingAction();

	// cTask을(를) 통해 상속됨
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual eState Run() override;
	virtual void RenderInfo() override;
private:
	weak_ptr<cActor> _actor;
	float _speed;
	bool _target;
};