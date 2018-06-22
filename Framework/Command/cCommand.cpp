#include "stdafx.h"
#include "cCommand.h"

byte cCommand::mKeyState[255];

void cCommand::GetKey(vector<byte>* keyCombo)
{
	//모든 키 상태 받아옴
	GetKeyboardState(mKeyState);
	for (auto& key : mKeyState)
	{
		//저장된 키를 다시 검사하면 스킵
		for (auto savedKey : *keyCombo)
		{
			if (key == savedKey)
				continue;
		}

		//누른 키를 배열에 저장
		if (key == (key & 0x80))
			keyCombo->push_back(key);
	}
}