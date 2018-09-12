#include "stdafx.h"
#include "cBehaviorTree.h"
#include "./Blackboard/cBlackboard.h"
#include "./Concrete/Component/BehaviorTree/cTaskFactory.h"

cBehaviorTree::cBehaviorTree(weak_ptr<cActor> actor)
	: _defaultName(L"New Behavior Tree")
	, _name(_defaultName)
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

	//test : 행동트리 테스트
	//_root->RenderName();
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
