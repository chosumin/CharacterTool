#include "stdafx.h"
#include "cBehaviorTree.h"
#include "./Concrete/Component/BehaviorTree/ConditionTasks.h"
#include "./Concrete/Component/BehaviorTree/ActionTasks.h"

cBehaviorTree::cBehaviorTree(weak_ptr<cActor> actor, int i)
	:_actor(actor)
{
	//todo : 행동 트리 툴 만들기

	_root = make_unique<cSelector>();
	
	if (i == 0)
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
	_root->AddChild(make_unique<cIdleAction>(actor));
}

cBehaviorTree::~cBehaviorTree()
{
}

void cBehaviorTree::Update()
{
	_root->Run();
}

void cBehaviorTree::ReadBinary(wstring file)
{
}
