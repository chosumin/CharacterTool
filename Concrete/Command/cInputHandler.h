#pragma once
#include "./cSingletone.h"

enum class eCommand
{
	MoveR, MoveL, MoveF, MoveB
};

class cCommand;
class cInputHandler : public cSingletone<cInputHandler>
{
private:
	friend class cSingletone<cInputHandler>;
public:
	void Move(eCommand command);

	void Undo();
	void Redo();

	void ShowKeyCombo();

	void PostRender();
private:
	cInputHandler();
	~cInputHandler();
private:
	void Insert(unique_ptr<cCommand> pCommand);
	void Destroy(int count);
private:
	map<eCommand, unique_ptr<cCommand>> mCommandInfo;

	vector<unique_ptr<cCommand>> mVecCommand;
	int mCurCount = -1;
};