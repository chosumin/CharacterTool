#include "stdafx.h"
#include "cKeyboard.h"

void cKeyboard::Update()
{
	CopyMemory(keyOldState, keyState, sizeof(keyOldState));

	ZeroMemory(keyState, sizeof(keyState));
	ZeroMemory(keyMap, sizeof(keyMap));

	GetKeyboardState(keyState);

	for (DWORD i = 0; i < MAX_INPUT_KEY; i++)
	{
		byte key = keyState[i] & 0x80;
		keyState[i] = key ? 1 : 0;

		int oldState = keyOldState[i];
		int state = keyState[i];

		if (oldState == 0 && state == 1)
			keyMap[i] = KEY_INPUT_STATUS_DOWN; //이전 0, 현재 1 - KeyDown
		else if (oldState == 1 && state == 0)
			keyMap[i] = KEY_INPUT_STATUS_UP; //이전 1, 현재 0 - KeyUp
		else if (oldState == 1 && state == 1)
			keyMap[i] = KEY_INPUT_STATUS_PRESS; //이전 1, 현재 1 - KeyPress
		else
			keyMap[i] = KEY_INPUT_STATUS_NONE;
	}
}

inline bool cKeyboard::Down(DWORD key) const
{
	return keyMap[key] == KEY_INPUT_STATUS_DOWN;
}

inline bool cKeyboard::Up(DWORD key) const
{
	return keyMap[key] == KEY_INPUT_STATUS_UP;
}

inline bool cKeyboard::Press(DWORD key) const
{
	return keyMap[key] == KEY_INPUT_STATUS_PRESS;
}

cKeyboard::cKeyboard()
{
	ZeroMemory(keyState, sizeof(keyState));
	ZeroMemory(keyOldState, sizeof(keyOldState));
	ZeroMemory(keyMap, sizeof(keyMap));
}

cKeyboard::~cKeyboard()
{

}
