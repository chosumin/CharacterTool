#include "stdafx.h"
#include "cBTreeRunner.h"
#include "./BehaviorTree/cBehaviorTree.h"
#include "./GameObject/cActor.h"
#include "./Blackboard/cBlackboard.h"

cBTreeRunner::cBTreeRunner(const weak_ptr<cBehaviorTree> & tree,
						   const ImVec2 & position,
						   weak_ptr<cTask> parent)
	: cTask(tree, "Tree Runner", position, parent)
{
}

cBTreeRunner::~cBTreeRunner()
{
}

std::unique_ptr<cTask> cBTreeRunner::Clone() const
{
	auto clone = make_unique<cBTreeRunner>(_tree, _pos);

	return move(clone);
}

cTask::eState cBTreeRunner::Run()
{
	auto treePtr = _tree.lock();
	if (!treePtr)
		return _state = eState::FAILURE;

	treePtr->Run();

	return _state = eState::SUCCESS;
}

void cBTreeRunner::RenderInfo()
{
	ImGui::TextColored(ActionTextColor, _taskName.c_str());

	ImGui::NewLine();
	
	ImGui::Text("Run!!!");
}

void cBTreeRunner::LoadJson(Json::Value & root)
{
	LoadDefaultInfo(root);

}

void cBTreeRunner::SaveJson(Json::Value & root)
{
	Json::Value task;

	SaveDefaultInfo(task);

	root.append(task);
}