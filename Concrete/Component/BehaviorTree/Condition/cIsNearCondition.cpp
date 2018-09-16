#include "stdafx.h"
#include "cIsNearCondition.h"
#include "./Blackboard/cBlackboard.h"
#include "./Helper/cMath.h"

cIsNearCondition::cIsNearCondition(weak_ptr<cBlackboard> blackboard,
							   weak_ptr<cBehaviorTree> tree,
							   const ImVec2 & position,
							   const weak_ptr<cTask>& parent)
	: cTask(tree, "Is Near?", position, parent)
	, _blackboard(blackboard)
	, _searchRange(0.0f)
{
}

cIsNearCondition::~cIsNearCondition()
{
}

std::unique_ptr<cTask> cIsNearCondition::Clone() const
{
	auto clone = make_unique<cIsNearCondition>(_blackboard, _tree, _pos, _parent);

	clone->_searchRange = _searchRange;

	return move(clone);
}

cTask::eState cIsNearCondition::Run()
{
	auto blackboardPtr = _blackboard.lock();
	if (!blackboardPtr)
		return _state = eState::NONE;

	float dist = blackboardPtr->GetFloat("DistanceGap");

	if (dist <= _searchRange)
		cDebug::RenewalLog("Find!");

	return _state = dist <= _searchRange ?
		eState::SUCCESS : eState::FAILURE;
}

void cIsNearCondition::RenderInfo()
{
	ImGui::TextColored(ImVec4(0, 255, 0, 1), _taskName.c_str());

	ImGui::NewLine();

	ImGui::Text("Search Range :");

	ImGui::InputFloat("", &_searchRange);
}

void cIsNearCondition::LoadJson(Json::Value & root)
{
	LoadDefaultInfo(root);

	Json::GetValue(root, "Search Range", _searchRange);
}

void cIsNearCondition::SaveJson(Json::Value & root)
{
	Json::Value task;

	SaveDefaultInfo(task);

	Json::SetValue(task, "Search Range", _searchRange);

	root.append(task);
}