#include "stdafx.h"
#include "cMouse.h"

cMouse::cMouse()
	:action(false)
{
	position = D3DXVECTOR3(0, 0, 0);
	prevPosition = D3DXVECTOR3(0, 0, 0);

	wheelStatus = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	wheelOldStatus = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	wheelMoveValue = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	ZeroMemory(buttonStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonOldStatus, sizeof(byte) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonMap, sizeof(byte) * MAX_INPUT_MOUSE);

	ZeroMemory(startDblClk, sizeof(DWORD) * MAX_INPUT_MOUSE);
	ZeroMemory(buttonCount, sizeof(int) * MAX_INPUT_MOUSE);

	timeDblClk = GetDoubleClickTime();
	startDblClk[0] = GetTickCount();

	for (int i = 1; i < MAX_INPUT_MOUSE; i++)
		startDblClk[i] = startDblClk[0];

	DWORD tLine = 0;
	SystemParametersInfo(SPI_GETWHEELSCROLLLINES, 0, &tLine, 0);
}

cMouse::~cMouse()
{
}

void cMouse::Update()
{
	//���� ���¿� ���ϱ� ���� ����
	CopyMemory(buttonOldStatus, buttonStatus, sizeof(buttonOldStatus));

	//�޸� �ʱ�ȭ
	ZeroMemory(buttonStatus, sizeof(buttonStatus));
	ZeroMemory(buttonMap, sizeof(buttonMap));

	//����, ������ Ŭ�� ����
	//�� Ŭ��, ȸ�� ����
	buttonStatus[0] = GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[1] = GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0;
	buttonStatus[2] = GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0;

	action = false;

	//�Է� ���� ����
	for (DWORD i = 0; i < MAX_INPUT_MOUSE; i++)
	{
		int tOldStatus = buttonOldStatus[i];
		int tStatus = buttonStatus[i];

		if (tOldStatus == 0 && tStatus == 1)
		{
			buttonMap[i] = BUTTON_INPUT_STATUS_DOWN;
			action = true;
		}
		else if (tOldStatus == 1 && tStatus == 0)
		{
			buttonMap[i] = BUTTON_INPUT_STATUS_UP;
			action = true;
		}
		else if (tOldStatus == 1 && tStatus == 1)
		{
			buttonMap[i] = BUTTON_INPUT_STATUS_PRESS;
			action = true;
		}
		else
			buttonMap[i] = BUTTON_INPUT_STATUS_NONE;
	}

	POINT point;
	GetCursorPos(&point);
	ScreenToClient(handle, &point); //���콺 ��ǥ �޾ƿ�

	wheelOldStatus.x = wheelStatus.x;
	wheelOldStatus.y = wheelStatus.y;

	wheelStatus.x = float(point.x);
	wheelStatus.y = float(point.y);

	//�� ȸ�� ���� ����
	wheelMoveValue = wheelStatus - wheelOldStatus;
	wheelOldStatus.z = wheelStatus.z;

	//���� �ð� üũ�ؼ� ����Ŭ�� üũ
	DWORD tButtonStatus = GetTickCount();
	for (DWORD i = 0; i < MAX_INPUT_MOUSE; i++)
	{
		if (buttonMap[i] == BUTTON_INPUT_STATUS_DOWN)
		{
			if (buttonCount[i] == 1)
			{
				if ((tButtonStatus - startDblClk[i]) >= timeDblClk)
					buttonCount[i] = 0;
			}
			buttonCount[i]++;

			if (buttonCount[i] == 1)
				startDblClk[i] = tButtonStatus;
		}

		if (buttonMap[i] == BUTTON_INPUT_STATUS_UP)
		{
			if (buttonCount[i] == 1)
			{
				if ((tButtonStatus - startDblClk[i]) >= timeDblClk)
					buttonCount[i] = 0;
			}
			else if (buttonCount[i] == 2)
			{
				if ((tButtonStatus - startDblClk[i]) <= timeDblClk)
					buttonMap[i] = BUTTON_INPUT_STATUS_DBLCLK;

				buttonCount[i] = 0;
			}
		}//if
	}//for(i)
}

LRESULT cMouse::WndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_LBUTTONDOWN || message == WM_MOUSEMOVE)
	{
		prevPosition = position;
		position.x = (float)LOWORD(lParam);
		position.y = (float)HIWORD(lParam);
	}

	if (message == WM_MOUSEWHEEL)
	{
		short tWheelValue = (short)HIWORD(wParam);

		wheelOldStatus.z = wheelStatus.z;
		wheelStatus.z += (float)tWheelValue;
	}

	return TRUE;
}

bool cMouse::Down(DWORD button) const
{
	return buttonMap[button] == BUTTON_INPUT_STATUS_DOWN;
}

bool cMouse::Up(DWORD button) const
{
	return buttonMap[button] == BUTTON_INPUT_STATUS_UP;
}

bool cMouse::Press(DWORD button) const
{
	return buttonMap[button] == BUTTON_INPUT_STATUS_PRESS;
}

bool cMouse::DbClick(DWORD button) const
{
	return buttonMap[button] == BUTTON_INPUT_STATUS_DBLCLK;
}

bool cMouse::WheelUp() const
{
	return wheelMoveValue.z > 0;
}

bool cMouse::WheelDown() const
{
	return wheelMoveValue.z < 0;
}
