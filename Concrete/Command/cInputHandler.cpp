#include "stdafx.h"
#include "cInputHandler.h"
#include "./Command/cCommand.h"
#include "./Actor/ActorMoveCommand.h"

cInputHandler::cInputHandler()
{

	mPlayerCommands[eCommand::MOVE_FORWARD] = sPlayerKey("Move Foward", 'W');
	mPlayerCommands[eCommand::MOVE_BACKWARD] = sPlayerKey("Move Backward", 'S');
	mPlayerCommands[eCommand::MOVE_LEFT] = sPlayerKey("Move Left", 'A');
	mPlayerCommands[eCommand::MOVE_RIGHT] = sPlayerKey("Move Right", 'D');

	mPlayerCommands[eCommand::DASH] = sPlayerKey("Dash", VK_LSHIFT);

	mPlayerCommands[eCommand::MELEE_ATTACK] = sPlayerKey("Melee Attack", VK_LBUTTON);
}

cInputHandler::~cInputHandler()
{
	Destroy(0);
}

void cInputHandler::Undo()
{
	//뒤로 돌아갈 명령이 없으면 실행 안함
	if (mVecCommand.empty() || mCurCount < 0)
		return;

	mVecCommand[mCurCount--]->Undo();

	cout << "명령 개수 : " << mVecCommand.size() << endl;
	cout << "현재 번호 : " << mCurCount << endl;
}

void cInputHandler::Redo()
{
	//Undo입력 없었으면 실행 안함
	if (static_cast<int>(mVecCommand.size() - 1) <= mCurCount)
		return;
	//명령 없었으면 실행 안함
	if (mVecCommand.empty())
		return;

	mVecCommand[++mCurCount]->Redo();

	cout << "명령 개수 : " << mVecCommand.size() << endl;
	cout << "현재 번호 : " << mCurCount << endl;
}

void cInputHandler::ShowKeyCombo()
{
	/*for (auto&& command : mPlayerCommands)
		command.second->ShowKeyCombo();*/
	//todo : 입력키 변경 화면
}

void cInputHandler::PostRender()
{
	ImGui::Begin("Command");
	{
		if (ImGui::Button("Insert"))
			Insert(make_unique<cRMoveCommand>());
		if (ImGui::Button("Undo"))
			Undo();
		if (ImGui::Button("Redo"))
			Redo();
	}
	ImGui::End();
}

bool cInputHandler::OnKeyEvent(eKeyEventType type, eCommand button)
{
	switch (type)
	{
		case eKeyEventType::DOWN:
			return cKeyboard::Get()->Down(mPlayerCommands[button].Key);
		case eKeyEventType::PRESS:
			return cKeyboard::Get()->Press(mPlayerCommands[button].Key);
		case eKeyEventType::UP:
			return cKeyboard::Get()->Up(mPlayerCommands[button].Key);
	}

	return false;
}

void cInputHandler::Insert(unique_ptr<cCommand> pCommand)
{
	//Undo 커맨드가 있을 경우, 노드 모두 삭제
	if (mCurCount < static_cast<int>(mVecCommand.size()))
		Destroy(mCurCount + 1);

	++mCurCount;
	mVecCommand.push_back(move(pCommand));

	cout << "명령 개수 : " << mVecCommand.size() << endl;
	cout << "현재 번호 : " << mCurCount << endl;
}

void cInputHandler::Destroy(int count)
{
	//명령 없었으면 종료
	if (mVecCommand.empty())
		return;

	auto iter = mVecCommand.begin() + count;
	while (iter != mVecCommand.end())
		iter = mVecCommand.erase(iter);
}