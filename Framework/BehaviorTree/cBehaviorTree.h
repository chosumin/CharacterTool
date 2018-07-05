#pragma once
#include "BaseTasks.h"

class cActor;
class cBehaviorTree
{
public:
	cBehaviorTree(weak_ptr<cActor> actor);
	~cBehaviorTree();

	void Update();
	void ReadBinary(wstring file);
private:
	unique_ptr<cSelector> _root;
	weak_ptr<cActor> _actor;
};