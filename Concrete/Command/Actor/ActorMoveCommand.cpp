#include "stdafx.h"
#include "ActorMoveCommand.h"
#include "./Command/cKeyboard.h"

string cRMoveCommand::mKeyName;
vector<byte> cRMoveCommand::mKeyCombo;

cRMoveCommand::cRMoveCommand()
{
	mKeyCombo.push_back(VK_RIGHT);
}

cRMoveCommand::~cRMoveCommand()
{
}

void cRMoveCommand::Execute(UINT time)
{
}

void cRMoveCommand::Undo()
{
}

void cRMoveCommand::Redo()
{
}

void cRMoveCommand::Change()
{
	mKeyCombo.clear();
	GetKey(&mKeyCombo);
}

void cRMoveCommand::ShowKeyCombo()
{
	//todo : ImGui·Î Ãâ·Â
}
