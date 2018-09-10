#include "stdafx.h"
#include "cBlackboard.h"
#include "./Command/cInputHandler.h"

cBlackboard::cBlackboard(weak_ptr<cActor> actor)
	: _speed(0.0f)
{
	_vector3s["Direction"] = { 0,0,0 }; //Ű���� ����
	_vector3s["TargetPos"] = { 0,0,0 }; //Ÿ�� ��ġ
	_vector3s["TargetDirection"] = { 0,0,0 }; //Ÿ�ٰ��� ����

	_bools["Dash"] = false; //�뽬 ����
	_bools["MeleeAttack"] = false; //���� ����
	_bools["Animate"] = false; //�ִϸ��̼� ����

	//�޺� ����
	//0 - ���� �ƴ�
	//1 - ���� ��
	//2 - �޺� ����
	_ints["AttackState"] = 0;
	_ints["AttackNumber"] = 0; //���� ���� ��ȣ
	_ints["MaxAttackNumber"] = 4; //�ִ� ���� ��ȣ
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
			case 0://���� �ƴ�
			{
				//���� ���·� ��ȯ
				_ints.at("AttackState") = 1;
				_bools.at("MeleeAttack") = true;
				return true;
			}
			break;
			case 3://�޺� ����
			{
				//�޺� ����
				_ints.at("AttackState") = 4;
				return true;
			}
			break;
		}
	}

	return false;
}
