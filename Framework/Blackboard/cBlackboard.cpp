#include "stdafx.h"
#include "cBlackboard.h"
#include "./Command/cInputHandler.h"

cBlackboard::cBlackboard(weak_ptr<cActor> actor)
	: _speed(0.0f)
	, _direction(0, 0, 0)
{
	//_bools[PRESS_MOVEKEY] = false;
}

cBlackboard::~cBlackboard()
{
}

bool cBlackboard::IsChange()
{
	bool moveEvent = OnMoveEvent();
	
	return moveEvent;
}

void cBlackboard::GetDirection(OUT D3DXVECTOR3 & direction)
{
	direction = _direction;
}

bool cBlackboard::OnMoveEvent()
{
	D3DXVECTOR3 direction = { 0,0,0 };

	if (cInputHandler::Get()->OnKeyEvent(eKeyEventType::PRESS, eCommand::MOVE_FORWARD))
		direction.z = 1.0f;
	else if (cInputHandler::Get()->OnKeyEvent(eKeyEventType::PRESS, eCommand::MOVE_BACKWARD))
		direction.z = -1.0f;

	if (cInputHandler::Get()->OnKeyEvent(eKeyEventType::PRESS, eCommand::MOVE_LEFT))
		direction.x = -1.0f;
	else if (cInputHandler::Get()->OnKeyEvent(eKeyEventType::PRESS, eCommand::MOVE_RIGHT))
		direction.x = 1.0f;

	if (cInputHandler::Get()->OnKeyEvent(eKeyEventType::UP, eCommand::MOVE_FORWARD))
		direction.z = -1.0f;
	else if (cInputHandler::Get()->OnKeyEvent(eKeyEventType::UP, eCommand::MOVE_BACKWARD))
		direction.z = 1.0f;

	if (cInputHandler::Get()->OnKeyEvent(eKeyEventType::UP, eCommand::MOVE_LEFT))
		direction.x = 1.0f;
	else if (cInputHandler::Get()->OnKeyEvent(eKeyEventType::UP, eCommand::MOVE_RIGHT))
		direction.x = -1.0f;
	
	D3DXVec3Normalize(&direction, &direction);

	if (fabs(_direction.x - direction.x) > D3DX_16F_EPSILON ||
		fabs(_direction.z - direction.z) > D3DX_16F_EPSILON)
	{
		_direction = direction;
		return true;
	}

	return false;
}
