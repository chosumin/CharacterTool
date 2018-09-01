#include "stdafx.h"
#include "cBehaviorTree.h"
#include "./Blackboard/cBlackboard.h"
#include "./Concrete/Component/BehaviorTree/Conditions.h"
#include "./Concrete/Component/BehaviorTree/Actions.h"

//test : 행동트리 테스트
#include "./GameObject/cActor.h"

cBehaviorTree::cBehaviorTree(weak_ptr<cActor> actor)
	:_actor(actor)
{
	_root = make_shared<cRootTask>();
	_root->SetPosition(ImVec2(350, 100));
}

cBehaviorTree::~cBehaviorTree()
{
}

void cBehaviorTree::Init()
{
	//test : 행동트리 테스트
	auto seq = make_shared<cCompositeTask>(shared_from_this(), ImVec2(0, 0), _root);
	auto cond = make_shared<cValueCompareCondition>(_actor.lock()->GetBlackboard(), shared_from_this(), ImVec2(0, 0), seq);
	seq->AddChild(cond);
	auto move1 = make_shared<cMovingAction>(_actor, shared_from_this(), ImVec2(0, 0), seq);
	seq->AddChild(move1);

	_root->AddChild(seq);

	auto move2 = make_shared<cMovingAction>(_actor, shared_from_this(), ImVec2(0, 0), seq);

	_root->AddChild(move2);
}

void cBehaviorTree::Run()
{
	//hack : 러닝 태스크도 초기화할지 말지 생각해보기
	_root->SetInitState(true);
	_root->Run();
}

void cBehaviorTree::Update()
{
	auto curTaskPtr = _currentTask.lock();
	if (curTaskPtr)
	{
		auto state = curTaskPtr->Run();
		if (state != cTask::eState::RUNNING)
		{
			auto parent = curTaskPtr->GetParent().lock();
			if(parent)
				parent->Run();
		}
	}
}

void cBehaviorTree::ReadBinary(wstring file)
{
}

weak_ptr<cRootTask> cBehaviorTree::GetRoot() const
{
	return _root;
}

void cBehaviorTree::SetCurrentTask(const weak_ptr<cTask> & task)
{
	_currentTask = task;
}
