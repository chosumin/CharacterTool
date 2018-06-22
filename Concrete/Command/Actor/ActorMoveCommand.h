#pragma once
#include "./Command/cCommand.h"

class cRMoveCommand : public cCommand
{
public:
	cRMoveCommand();
	~cRMoveCommand();

	void Execute(UINT time);

	// Command을(를) 통해 상속됨
	virtual void Undo() override;
	virtual void Redo() override;
	virtual void Change() override;
	virtual void ShowKeyCombo() override;
private:
	static string mKeyName;
	static vector<byte> mKeyCombo;
private:
	UINT mTime;
};