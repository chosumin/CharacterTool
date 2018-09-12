#include "stdafx.h"
#include "Decorators.h"

cDecorator::cDecorator(const weak_ptr<cBehaviorTree>& tree, const ImVec2 & position, weak_ptr<cTask> parent)
	:cTask(tree, "Decorator", position, parent)
{
}

cDecorator::~cDecorator()
{
}

void cDecorator::RenderInfo()
{
	ImGui::TextColored(ImVec4(128, 0, 255, 255), "%s", _taskName.c_str());
}

void cDecorator::AddChild(std::shared_ptr<cTask> child)
{
	if (!_child.empty())
		_child.clear();

	_child.emplace_back(move(child));
}

cTask::TaskList * cDecorator::GetChildren()
{
	return &_child;
}

void cDecorator::LoadJson(Json::Value & root)
{
	LoadDefaultInfo(root);
}

void cDecorator::SaveJson(Json::Value & root)
{
	Json::Value task;

	SaveDefaultInfo(task);

	Json::Value childValue;

	UINT size = _child.size();
	for (auto&& child : _child)
		child->SaveJson(childValue);

	task["Children"] = childValue;

	root.append(task);
}

/******************************************************************/

cInverter::cInverter(const weak_ptr<cBehaviorTree>& tree, const ImVec2 & position, weak_ptr<cTask> parent)
	:cDecorator(tree, position, parent)
{
	_taskName = "Inverter";
}

cInverter::~cInverter()
{
}

std::unique_ptr<cTask> cInverter::Clone() const
{
	auto clone = make_unique<cInverter>(_tree, _pos, _parent);
	
	for (auto&& child : _child)
	{
		auto childClone = child->Clone();
		clone->AddChild(move(childClone));
	}

	return move(clone);
}

cTask::eState cInverter::Run()
{
	if (!_child.empty())
	{
		_state = _child[0]->Run();

		if (_state == eState::RUNNING)
			return _state;

		if (_state == eState::NONE)
			return _state;
	}

	return 	_state = _state == eState::SUCCESS ? eState::FAILURE : eState::SUCCESS;
}

/******************************************************************/

cSucceeder::cSucceeder(const weak_ptr<cBehaviorTree>& tree, const ImVec2 & position, weak_ptr<cTask> parent)
	:cDecorator(tree, position, parent)
{
	_taskName = "Succeeder";
}

cSucceeder::~cSucceeder()
{
}

std::unique_ptr<cTask> cSucceeder::Clone() const
{
	auto clone = make_unique<cSucceeder>(_tree, _pos, _parent);
	for (auto&& child : _child)
	{
		auto childClone = child->Clone();
		clone->AddChild(move(childClone));
	}
	return move(clone);
}

cTask::eState cSucceeder::Run()
{
	if (!_child.empty())
	{
		_child[0]->Run();
	}

	return _state = eState::SUCCESS;
}
