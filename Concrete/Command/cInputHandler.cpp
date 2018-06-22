#include "stdafx.h"
#include "cInputHandler.h"
#include "./Command/cCommand.h"
#include "./Actor/ActorMoveCommand.h"

cInputHandler::cInputHandler()
{
	mCommandInfo[eCommand::MoveR] = make_unique<cRMoveCommand>();
}

cInputHandler::~cInputHandler()
{
	Destroy(0);
}

void cInputHandler::Move(eCommand command)
{
	//todo : �̵� ��� ����
	//todo : �̵� ��� ����
	//todo : Insert(pCommand);
}

void cInputHandler::Undo()
{
	//�ڷ� ���ư� ����� ������ ���� ����
	if (mVecCommand.empty() || mCurCount < 0)
		return;

	mVecCommand[mCurCount--]->Undo();

	cout << "��� ���� : " << mVecCommand.size() << endl;
	cout << "���� ��ȣ : " << mCurCount << endl;
}

void cInputHandler::Redo()
{
	//Undo�Է� �������� ���� ����
	if (static_cast<int>(mVecCommand.size() - 1) <= mCurCount)
		return;
	//��� �������� ���� ����
	if (mVecCommand.empty())
		return;

	mVecCommand[++mCurCount]->Redo();

	cout << "��� ���� : " << mVecCommand.size() << endl;
	cout << "���� ��ȣ : " << mCurCount << endl;
}

void cInputHandler::ShowKeyCombo()
{
	for (auto&& command : mCommandInfo)
		command.second->ShowKeyCombo();
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

void cInputHandler::Insert(unique_ptr<cCommand> pCommand)
{
	//Undo Ŀ�ǵ尡 ���� ���, ��� ��� ����
	if (mCurCount < static_cast<int>(mVecCommand.size()))
		Destroy(mCurCount + 1);

	++mCurCount;
	mVecCommand.push_back(move(pCommand));

	cout << "��� ���� : " << mVecCommand.size() << endl;
	cout << "���� ��ȣ : " << mCurCount << endl;
}

void cInputHandler::Destroy(int count)
{
	//��� �������� ����
	if (mVecCommand.empty())
		return;

	auto iter = mVecCommand.begin() + count;
	while (iter != mVecCommand.end())
		iter = mVecCommand.erase(iter);
}