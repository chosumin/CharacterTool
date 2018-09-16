#pragma once
#define MAX_INPUT_MOUSE 8
#include "./Common/cSingletone.h"

class cMouse : public cSingletone<cMouse>
{
private:
	friend class cSingletone<cMouse>;
public:
	void		Update();
	LRESULT		WndProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	void		SetHandle(HWND handle)	{ this->handle = handle; }
	D3DXVECTOR3 GetPosition() const		{ return position; }
	D3DXVECTOR3 GetDelta() const { return position - prevPosition; }
	D3DXVECTOR3 GetMoveValue() const	{ return wheelMoveValue; }
	void SetMouseValue(const D3DXVECTOR3& value)
	{
		wheelMoveValue = value;
	}

	bool OnAnyKeyEvent() { return action; }
public:
	bool Down(DWORD button) const;
	bool Up(DWORD button) const;
	bool Press(DWORD button) const;
	bool DbClick(DWORD button) const;
	bool WheelUp() const;
	bool WheelDown() const;
private:
	cMouse();
	~cMouse();
private:
	bool		action;

	HWND		handle;

	D3DXVECTOR3 prevPosition;
	D3DXVECTOR3 position; //마우스 위치

	byte		buttonStatus[MAX_INPUT_MOUSE];
	byte		buttonOldStatus[MAX_INPUT_MOUSE];
	byte		buttonMap[MAX_INPUT_MOUSE];

	DWORD		timeDblClk;
	DWORD		startDblClk[MAX_INPUT_MOUSE];
	int			buttonCount[MAX_INPUT_MOUSE];

	D3DXVECTOR3 wheelStatus;
	D3DXVECTOR3 wheelOldStatus;
	D3DXVECTOR3 wheelMoveValue;
private:
	enum
	{
		cMouse_ROTATION_NONE = 0,
		cMouse_ROTATION_LEFT,
		cMouse_ROTATION_RIGHT
	};

	enum
	{
		BUTTON_INPUT_STATUS_NONE = 0,
		BUTTON_INPUT_STATUS_DOWN,
		BUTTON_INPUT_STATUS_UP,
		BUTTON_INPUT_STATUS_PRESS,
		BUTTON_INPUT_STATUS_DBLCLK
	};
};

