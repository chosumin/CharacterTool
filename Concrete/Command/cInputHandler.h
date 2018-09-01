#pragma once
#include "./cSingletone.h"

enum class eCommand
{
	MOVE_FORWARD, MOVE_BACKWARD, MOVE_LEFT, MOVE_RIGHT,
	MELEE_ATTACK,
	COUNT,
};

enum class eKeyEventType
{
	DOWN, PRESS, UP,
};

struct sPlayerKey
{
	string Name;
	DWORD Key;

	sPlayerKey() {}
	sPlayerKey(string name, DWORD key)
		:Name(name), Key(key) {}
};

class cCommand;
class cInputHandler : public cSingletone<cInputHandler>
{
private:
	friend class cSingletone<cInputHandler>;
public:
	void Undo();
	void Redo();

	void ShowKeyCombo();

	void PostRender();

	//해당 키에 대한 이벤트 발생 확인
	//@param : 키보드, 마우스 - 다운, 프레스, 업
	//@param : 조사할 키
	bool OnKeyEvent(eKeyEventType type, eCommand button);

	//등록한 키 중 하나라도 반응했는지 확인
	bool OnAnyKeyEvent() { return cKeyboard::Get()->OnAnyKeyEvent() || cMouse::Get()->OnAnyKeyEvent(); }

	const map<eCommand, sPlayerKey>& GetKeyList() const
	{ return mPlayerCommands; }

	const DWORD GetCommand(eCommand command)
	{
		return mPlayerCommands[command].Key;
	}
private:
	cInputHandler();
	~cInputHandler();
private:
	void Insert(unique_ptr<cCommand> pCommand);
	void Destroy(int count);
private:
	vector<unique_ptr<cCommand>> mVecCommand;
	int mCurCount = -1;

	map<eCommand, sPlayerKey> mPlayerCommands;
};