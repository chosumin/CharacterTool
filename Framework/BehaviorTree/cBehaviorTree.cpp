#include "stdafx.h"
#include "cBehaviorTree.h"
#include "./Concrete/Component/BehaviorTree/Conditions.h"
#include "./Concrete/Component/BehaviorTree/Actions.h"

cBehaviorTree::cBehaviorTree(weak_ptr<cActor> actor)
	:_actor(actor)
{
	_root = make_shared<cSelector>("Root");
	_root->SetPosition(ImVec2(350, 100));
	/*if (i == 0)
	{
		auto attackSeq = make_unique<cSequence>();
		attackSeq->AddChild(make_unique<cAttackKeyPress>(actor));
		attackSeq->AddChild(make_unique<cAttackAction>(actor));
		_root->AddChild(move(attackSeq));
		auto moveSel = make_unique<cSelector>();
		auto seq1 = make_unique<cSequence>();
		seq1->AddChild(make_unique<cSideKeyPress>(actor));
		seq1->AddChild(make_unique<cUpKeyPress>(actor));
		moveSel->AddChild(move(seq1));
		auto sel1 = make_unique<cSelector>();
		sel1->AddChild(make_unique<cUpKeyPress>(actor));
		sel1->AddChild(make_unique<cSideKeyPress>(actor));
		moveSel->AddChild(move(sel1));
		_root->AddChild(move(moveSel));
	}
	else
	{
		_root->AddChild(make_unique <cDamageAction>(actor));
	}
	_root->AddChild(make_unique<cIdleAction>(actor));*/
}

cBehaviorTree::~cBehaviorTree()
{
}

void cBehaviorTree::Run()
{
	_root->Run();
}

void cBehaviorTree::Update()
{
	auto curTask = _currentTask.lock();
	if (curTask)
		curTask->Update();
}

void cBehaviorTree::ReadBinary(wstring file)
{
}

weak_ptr<cSelector> cBehaviorTree::GetRoot() const
{
	return _root;
}

void cBehaviorTree::SetCurrentTask(weak_ptr<cTask> task)
{
	_currentTask = task;
}
