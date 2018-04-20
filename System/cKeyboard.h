#pragma once
#define MAX_INPUT_KEY 255

class cKeyboard : public cSingletone<cKeyboard>
{
public:
	void Update();
public:
	inline bool Down(DWORD key) const;
	inline bool Up(DWORD key) const;
	inline bool Press(DWORD key) const;
private:
	cKeyboard();
	~cKeyboard();
private:
	byte keyState[MAX_INPUT_KEY];
	byte keyOldState[MAX_INPUT_KEY];
	byte keyMap[MAX_INPUT_KEY];
private:
	enum
	{
		KEY_INPUT_STATUS_NONE = 0,
		KEY_INPUT_STATUS_DOWN,
		KEY_INPUT_STATUS_UP,
		KEY_INPUT_STATUS_PRESS,
	};

	friend class cSingletone<cKeyboard>;
};