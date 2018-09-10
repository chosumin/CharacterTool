#include "stdafx.h"
#include "cBlackboard.h"
#include "./Command/cInputHandler.h"

cBlackboard::cBlackboard(weak_ptr<cActor> actor)
	: _speed(0.0f)
{
	_vector3s["Direction"] = { 0,0,0 }; //키보드 방향
	_vector3s["TargetPos"] = { 0,0,0 }; //타겟 위치
	_vector3s["TargetDirection"] = { 0,0,0 }; //타겟과의 방향

	_bools["Dash"] = false; //대쉬 여부
	_bools["MeleeAttack"] = false; //공격 여부
	_bools["Animate"] = false; //애니메이션 여부

	//콤보 상태
	//0 - 공격 아님
	//1 - 공격 중
	//2 - 콤보 가능
	_ints["AttackState"] = 0;
	_ints["AttackNumber"] = 0; //현재 공격 번호
	_ints["MaxAttackNumber"] = 4; //최대 공격 번호
}

cBlackboard::~cBlackboard()
{
}

bool cBlackboard::IsChange()
{
	static DWORD update = timeGetTime();

	bool onEvent = false;

	/*if (timeGetTime() - update >= 800)
	{
		onEvent = true;
		update = timeGetTime();
	}*/

	onEvent |= OnMoveEvent();
	onEvent |= OnDashEvent();
	onEvent |= OnAttackEvent();

	return onEvent;
}

bool cBlackboard::OnMoveEvent()
{
	D3DXVECTOR3 direction = { 0,0,0 };

	OnMoveKeyEvent(direction, 1.0f, eKeyEventType::PRESS);
	OnMoveKeyEvent(direction, 0.0f, eKeyEventType::UP);

	D3DXVec3Normalize(&direction, &direction);

	auto& actorDirection = _vector3s["Direction"];
	if (fabs(actorDirection.x - direction.x) > D3DX_16F_EPSILON ||
		fabs(actorDirection.z - direction.z) > D3DX_16F_EPSILON)
	{
		actorDirection = direction;
		return true;
	}

	return false;
}

void cBlackboard::OnMoveKeyEvent(D3DXVECTOR3 & direction, float value, eKeyEventType type)
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

bool cBlackboard::OnDashEvent()
{
	bool dash = _bools["Dash"];

	if (cInputHandler::Get()->OnKeyEvent(eKeyEventType::PRESS, eCommand::DASH))
		dash = true;
	else if (cInputHandler::Get()->OnKeyEvent(eKeyEventType::UP, eCommand::DASH))
		dash = false;

	if (_bools["Dash"] != dash)
	{
		_bools["Dash"] = dash;
		return true;
	}

	return false;
}

bool cBlackboard::OnAttackEvent()
{
	if (cInputHandler::Get()->OnKeyEvent(eKeyEventType::DOWN, eCommand::MELEE_ATTACK))
	{
		switch (_ints.at("AttackState"))
		{
			case 0://공격 아님
			{
				//공격 상태로 전환
				_ints.at("AttackState") = 1;
				_bools.at("MeleeAttack") = true;
				return true;
			}
			break;
			case 3://콤보 가능
			{
				//콤보 성공
				_ints.at("AttackState") = 4;
				return true;
			}
			break;
		}
	}

	return false;
}
