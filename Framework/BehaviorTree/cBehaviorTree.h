#pragma once
#include "BaseTasks.h"

class cActor;
class cBehaviorTree
{
public:
	cBehaviorTree(weak_ptr<cActor> actor);
	~cBehaviorTree();

	//Ʈ�� ��ȸ�ϸ� �½�ũ �˻�, �̺�Ʈ
	void Run();

	void Update();

	void ReadBinary(wstring file);

	weak_ptr<cSelector> GetRoot() const;

	void SetCurrentTask(weak_ptr<cTask> task);
private:
	shared_ptr<cSelector> _root;
	weak_ptr<cActor> _actor;

	weak_ptr<cTask> _currentTask;
};