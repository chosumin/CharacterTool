#include "stdafx.h"
#include "cCommand.h"

byte cCommand::mKeyState[255];

void cCommand::GetKey(vector<byte>* keyCombo)
{
	//��� Ű ���� �޾ƿ�
	GetKeyboardState(mKeyState);
	for (auto& key : mKeyState)
	{
		//����� Ű�� �ٽ� �˻��ϸ� ��ŵ
		for (auto savedKey : *keyCombo)
		{
			if (key == savedKey)
				continue;
		}

		//���� Ű�� �迭�� ����
		if (key == (key & 0x80))
			keyCombo->push_back(key);
	}
}