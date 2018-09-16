#include "stdafx.h"
#include "cUserInspector.h"
#include "./Command/cInputHandler.h"
#include "./GameObject/cActor.h"

cUserInspector::cUserInspector(weak_ptr<cBlackboard> blackboard)
	: _blackboard(blackboard)
{
}

cUserInspector::~cUserInspector()
{
}

bool cUserInspector::IsChange()
{
	//SetPositionData();

	//static DWORD update = timeGetTime();
	/*if (timeGetTime() - update >= 800)
	{
	onEvent = true;
	update = timeGetTime();
	}*/

	bool onEvent = false;

	onEvent |= OnMoveEvent();
	onEvent |= OnDashEvent();
	onEvent |= OnAttackEvent();

	return onEvent;
}

bool cUserInspector::OnMoveEvent()
{
	auto blackboardPtr = _blackboard.lock();

	D3DXVECTOR3 direction = { 0,0,0 };

	OnMoveKeyEvent(direction, 1.0f, eKeyEventType::PRESS);
	OnMoveKeyEvent(direction, 0.0f, eKeyEventType::UP);

	D3DXVec3Normalize(&direction, &direction);

	auto& actorDirection = blackboardPtr->GetVector3("Direction");
	if (fabs(actorDirection.x - direction.x) > D3DX_16F_EPSILON ||
		fabs(actorDirection.z - direction.z) > D3DX_16F_EPSILON)
	{
		blackboardPtr->SetVector3("Direction", direction);
		return true;
	}

	return false;
}

void cUserInspector::OnMoveKeyEvent(D3DXVECTOR3 & direction, float value, eKeyEventType type)
{
	if (cInputHandler::Get()->OnKeyEvent(type, eCommand::MOVE_FORWARD))
		direction.z = value;
	else if (cInputHandler::Get()->OnKeyEvent(type, eCommand::MOVE_BACKWARD))
		direction.z = -value;

	if (cInputHandler::Get()->OnKeyEvent(type, eCommand::MOVE_LEFT))
		direction.x = -value;
	else if (cInputHandler::Get()->OnKeyEvent(type, eCommand::MOVE_RIGHT))
		direction.x = value;
}

bool cUserInspector::OnDashEvent()
{
	auto blackboardPtr = _blackboard.lock();

	bool dash = blackboardPtr->GetBool("Dash");

	if (cInputHandler::Get()->OnKeyEvent(eKeyEventType::PRESS, eCommand::DASH))
		dash = true;
	else if (cInputHandler::Get()->OnKeyEvent(eKeyEventType::UP, eCommand::DASH))
		dash = false;

	if (blackboardPtr->GetBool("Dash") != dash)
	{
		blackboardPtr->SetBool("Dash", dash);
		return true;
	}

	return false;
}

bool cUserInspector::OnAttackEvent()
{
	if (cInputHandler::Get()->OnKeyEvent(eKeyEventType::DOWN, eCommand::MELEE_ATTACK))
	{
		auto blackboardPtr = _blackboard.lock();

		switch (blackboardPtr->GetInt("AttackState"))
		{
			case 0://공격 아님
			{
				//공격 상태로 전환
				blackboardPtr->SetInt("AttackState", 1);
				blackboardPtr->SetBool("MeleeAttack", true);
				return true;
			}
			break;
			case 3://콤보 가능
			{
				//콤보 성공
				blackboardPtr->SetInt("AttackState", 4);
				return true;
			}
			break;
		}
	}

	return false;
}

void cUserInspector::SetPositionData()
{
	auto boardPtr = _blackboard.lock();
	if (!boardPtr)
		return;

	//적의 위치 세팅
	D3DXVECTOR3 targetPos;
	auto enemyPtr = boardPtr->GetEnemy().lock();
	if (enemyPtr)
	{
		enemyPtr->GetPosition(targetPos);
		boardPtr->SetVector3("TargetPos", targetPos);
	}

	//자신의 위치 세팅
	D3DXVECTOR3 myPos;
	auto mePtr = boardPtr->GetActor().lock();
	mePtr->GetPosition(myPos);
	boardPtr->SetVector3("MyPos", myPos);

	D3DXVECTOR3 gap = targetPos - myPos;
	D3DXVec3Normalize(&gap, &gap);
	boardPtr->SetVector3("TargetDirection", gap);
}
