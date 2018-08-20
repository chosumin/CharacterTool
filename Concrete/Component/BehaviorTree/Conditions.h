#pragma once
#include "./BehaviorTree/BaseTasks.h"

class cSideKeyPress : public cTask
{
public:
	cSideKeyPress(weak_ptr<cActor> actor);
	~cSideKeyPress();

	// cTask��(��) ���� ��ӵ�
	virtual bool Run() override;
private:
	weak_ptr<cActor> _actor;
};

/*********************************************************/

class cUpKeyPress : public cTask
{
public:
	cUpKeyPress(weak_ptr<cActor> actor);
	~cUpKeyPress();

	// cTask��(��) ���� ��ӵ�
	virtual bool Run() override;
private:
	weak_ptr<cActor> _actor;
};

/*********************************************************/

class cIdleAction : public cTask
{
public:
	cIdleAction(weak_ptr<cActor> actor);
	~cIdleAction();

	// cTask��(��) ���� ��ӵ�
	virtual bool Run() override;
private:
	weak_ptr<cActor> _actor;
};

/*********************************************************/

class cAttackKeyPress : public cTask
{
public:
	cAttackKeyPress(weak_ptr<cActor> actor);
	~cAttackKeyPress();

	// cTask��(��) ���� ��ӵ�
	virtual bool Run() override;
private:
	weak_ptr<cActor> _actor;
};

/*********************************************************/

class cAnimIsEnd : public cTask
{
public:
	cAnimIsEnd(weak_ptr<cActor> actor, UINT animIndex);
	~cAnimIsEnd();

	// cTask��(��) ���� ��ӵ�
	virtual bool Run() override;
private:
	weak_ptr<cActor> _actor;
	UINT _animIndex;
};

/*********************************************************/

class cDamageAction : public cTask
{
public:
	cDamageAction(weak_ptr<cActor> actor);
	~cDamageAction();

	// cTask��(��) ���� ��ӵ�
	virtual bool Run() override;
private:
	weak_ptr<cActor> _actor;
};