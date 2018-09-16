#include "stdafx.h"
#include "cBehaviorTree.h"
#include "./Blackboard/cBlackboard.h"
#include "./Concrete/Component/BehaviorTree/cTaskFactory.h"

cBehaviorTree::cBehaviorTree(weak_ptr<cActor> actor)
	: _defaultName(L"New Behavior Tree")
	, _name(L"New Behavior Tree")
	, _actor(actor)
{
	_root = make_shared<cRootTask>();
	_root->SetPosition(ImVec2(350, 100));
}

cBehaviorTree::~cBehaviorTree()
{
}

void cBehaviorTree::Init()
{
}

void cBehaviorTree::Run()
{
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
			/*auto parentPtr = curTaskPtr->GetParent().lock();
			if (parentPtr)
				parentPtr->Run();*/
			UpdateParent(curTaskPtr->GetParent());
		}
	}
}

weak_ptr<cRootTask> cBehaviorTree::GetRoot() const
{
	return _root;
}

void cBehaviorTree::SetCurrentTask(const weak_ptr<cTask> & task)
{
	_currentTask = task;
}

void cBehaviorTree::LoadJson(Json::Value & root)
{
	_root->GetChildren()->clear();

	Json::Value rootValue = root[0];
	_root->LoadJson(rootValue);

	cTaskFactory factory;
	factory.CreateTask(_root, rootValue["Children"], _actor);
}

void cBehaviorTree::SaveJson(Json::Value & root)
{
	_root->SaveJson(root);
}

void cBehaviorTree::UpdateParent(const weak_ptr<cTask>& parent)
{
	auto parentPtr = parent.lock();
	if (parentPtr)
	{
		auto state = parentPtr->Run();
		if (state != cTask::eState::RUNNING)
		{
			UpdateParent(parentPtr->GetParent());
		}
	}
}
