#include "stdafx.h"
#include "cFreePointCamera.h"
#include "./Command/cKeyboard.h"

cFreePointCamera::cFreePointCamera(float fMoveSpeed, float fRotationSpeed)
	:m_fMoveSpeed(fMoveSpeed), m_fRotationSpeed(fRotationSpeed)
{
}

cFreePointCamera::~cFreePointCamera()
{
}

void cFreePointCamera::Update()
{
	D3DXVECTOR3 vDirection{ 0,0,0 };

	if (cKeyboard::Get()->Press('W'))
		vDirection.x = 1.0f;
	else if (cKeyboard::Get()->Press('S'))
		vDirection.x = -1.0f;

	if (cKeyboard::Get()->Press('A'))
		vDirection.y = -1.0f;
	else if (cKeyboard::Get()->Press('D'))
		vDirection.y = 1.0f;

	if (cKeyboard::Get()->Press('E'))
		vDirection.z = 1.0f;
	else if (cKeyboard::Get()->Press('Q'))
		vDirection.z = -1.0f;

	Move(vDirection, m_fMoveSpeed);

	if (cMouse::Get()->Press(1))
	{
		auto vMove = cMouse::Get()->GetMoveValue();
		Rotate(vMove, m_fRotationSpeed);
	}

	UpdateView();
}

void cFreePointCamera::PostRender()
{
	//todo : 회전 스피드, 이동 스피드 조절
}