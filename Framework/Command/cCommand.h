#pragma once

class cCommand
{
private:
	friend class InputHandler;
public:
	virtual ~cCommand()
	{
		cout << "Å° ÇØÁ¦" << endl;
	}

	virtual void Undo() = 0;
	virtual void Redo() = 0;
	virtual void Change() = 0;
	virtual void ShowKeyCombo() = 0;
protected:
	static void GetKey(vector<byte>* keyCombo);
protected:
	static byte mKeyState[255];
};