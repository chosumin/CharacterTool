#include "stdafx.h"
#include "ActionTasks.h"
#include "./GameObject/cActor.h"

cAttackAction::cAttackAction(weak_ptr<cActor> actor)
	:cTask(typeid(this).name()),
	_actor(actor)
{
}

cAttackAction::~cAttackAction()
{
}

bool cAttackAction::Run()
{
	_actor.lock()->Attack();
	return true;
}
