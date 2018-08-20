#pragma once
#include "./BehaviorTree/BaseTasks.h"

enum class eKeyType
{
	MoveForward, MoveBackward, MoveLeft, MoveRight,
	MeleeAttack,
	Count,
};

class cActor;
class cKeyCondition : public cTask
{
public:
	enum class eKeyType
	{
		DOWN, PRESS, UP
	};
public:
	cKeyCondition(weak_ptr<cBehaviorTree> tree, const ImVec2& position);
	~cKeyCondition();

	// cTask을(를) 통해 상속됨
	virtual std::unique_ptr<cTask> Clone() const override;
	virtual bool Run() override;
	virtual void Update() override;
	virtual void RenderInfo() override;
private:
	cKeyCondition::eKeyType _type;
	string _selectedButton;
	DWORD _button;
};