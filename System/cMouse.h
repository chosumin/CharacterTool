#pragma once
#define MAX_INPUT_MOUSE 8

class cMouse : public cSingletone<cMouse>
{
public:
	void		Update();
	LRESULT		WndProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	void		SetHandle(HWND handle)	{ this->handle = handle; }
	D3DXVECTOR3 GetPosition() const		{ return position; }
	D3DXVECTOR3 GetMoveValue() const	{ return wheelMoveValue; }
public:
	inline bool Down(DWORD button) const;
	inline bool Up(DWORD button) const;
	inline bool Press(DWORD button) const;
	inline bool DbClick(DWORD button) const;
	inline bool WheelUp() const;
	inline bool WheelDown() const;
private:
	cMouse();
	~cMouse();
private:
	HWND		handle;
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

	friend class cSingletone<cMouse>;
};

