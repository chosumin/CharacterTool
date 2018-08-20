#include "stdafx.h"
#include "Conditions.h"
#include "./GameObject/cActor.h"
#include "./Interface/iAction.h"

cSideKeyPress::cSideKeyPress(weak_ptr<cActor> actor)
	:cTask(typeid(this).name()),
	_actor(actor)
{
}

cSideKeyPress::~cSideKeyPress()
{
}

bool cSideKeyPress::Run()
{
	//hack : 키를 매개변수로 받기
	auto actorPtr = _actor.lock();
	auto delta = cFrame::Delta() * 100.0f;

	bool isPress = false;
	if (cKeyboard::Get()->Press(VK_RIGHT))
	{
		actorPtr->Rotate(delta);
		isPress = true;
	}

	if (cKeyboard::Get()->Press(VK_LEFT))
	{
		actorPtr->Rotate(-delta);
		isPress = true;
	}

	return isPress;
}

/*********************************************************/

cUpKeyPress::cUpKeyPress(weak_ptr<cActor> actor)
	:cTask(typeid(this).name()),
	_actor(actor)
{
}

cUpKeyPress::~cUpKeyPress()
{
}

bool cUpKeyPress::Run()
{
	auto actorPtr = _actor.lock();
	if (cKeyboard::Get()->Press(VK_UP))
	{
		actorPtr->Move({ 0,0,1 }, cFrame::Delta() * 10.0f);
		return true;
	}

	return false;
}

/*********************************************************/

cIdleAction::cIdleAction(weak_ptr<cActor> actor)
	:cTask(typeid(this).name()),
	_actor(actor)
{
}

cIdleAction::~cIdleAction()
{
}

bool cIdleAction::Run()
{
	_actor.lock()->Idle();
	return true;
}

/*********************************************************/

cAttackKeyPress::cAttackKeyPress(weak_ptr<cActor> actor)
	:cTask(typeid(this).name()),
	_actor(actor)
{
}

cAttackKeyPress::~cAttackKeyPress()
{
}

bool cAttackKeyPress::Run()
{
	if (cKeyboard::Get()->Down(VK_SPACE))
	{
		_actor.lock()->time = timeGetTime();
		_actor.lock()->isAttack = true;
	}
	return _actor.lock()->isAttack;
}

/*********************************************************/

cAnimIsEnd::cAnimIsEnd(weak_ptr<cActor> actor, UINT animIndex)
	:cTask(typeid(this).name()),
	_actor(actor),
	_animIndex(animIndex)
{
}

cAnimIsEnd::~cAnimIsEnd()
{
}

bool cAnimIsEnd::Run()
{
	//찾는 애니메이션이 실행 중이지 않으면 true
	return (_actor.lock()->GetCurrentAnim() != _animIndex);
}

/*********************************************************/

cDamageAction::cDamageAction(weak_ptr<cActor> actor)
	:cTask(typeid(this).name()),
	_actor(actor)
{
}

cDamageAction::~cDamageAction()
{
}

bool cDamageAction::Run()
{
	if (_actor.lock()->isDamage)
	{
		_actor.lock()->Damage();
		return true;
	}
	return false;
}