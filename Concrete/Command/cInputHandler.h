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

	//�ش� Ű�� ���� �̺�Ʈ �߻� Ȯ��
	//@param : Ű����, ���콺 - �ٿ�, ������, ��
	//@param : ������ Ű
	bool OnKeyEvent(eKeyEventType type, eCommand button);

	//����� Ű �� �ϳ��� �����ߴ��� Ȯ��
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