#include "stdafx.h"
#include "BaseTasks.h"

cTask::cTask(string name, ImVec2& position)
	: _taskName(name)
	, _pos(position)
	, _delete(eTaskState::NONE)
{
}

bool cTask::RenderMenu()
{
	//노드 삭제 메뉴
	if (ImGui::MenuItem("Delete All", nullptr, false, true))
	{
		_delete = eTaskState::DELETE_ALL;
		return true;
	}

	if (ImGui::MenuItem("Delete Only Me", nullptr, false, true))
	{
		_delete = eTaskState::DELETE_ONLY_ME;
		return true;
	}
	
	if (ImGui::MenuItem("Delete Only SubNodes", nullptr, false, true))
	{
		_delete = eTaskState::DELETE_ONLY_SUBNODES;
		return true;
	}

	//노드 복사 메뉴
	if (ImGui::MenuItem("Copy Only Me", nullptr, false, true))
	{
		auto task = Clone();
		return true;
	}

	//노드 복사 메뉴
	if (ImGui::MenuItem("Copy With SubNodes", nullptr, false, true))
	{
		auto task = Clone();
		return true;
	}

	return false;
}

cTask::TaskList * cTask::GetChildren()
{
	return nullptr;
}

/**********************************************************/

cCompositeTask::cCompositeTask(string name)
	: cTask(name)
{
}

cCompositeTask::~cCompositeTask()
{
}

cCompositeTask::TaskList * cCompositeTask::GetChildren()
{
	return &_children;
}

void cCompositeTask::RenderInfo()
{
	ImGui::Text("%s", GetName().c_str());
}

void cCompositeTask::AddChild(std::shared_ptr<cTask> child)
{
	_children.emplace_back(move(child));
}

std::unique_ptr<cTask> cCompositeTask::Clone() const
{
	//todo : 복사
	return nullptr;
}

/**********************************************************/

cSelector::cSelector(ImVec2& position)
	: cCompositeTask("Selector")
{
	_pos = position;
}

cSelector::cSelector(string name)
	:cCompositeTask(name)
{
}

bool cSelector::Run()
{
	for (auto&& child : *GetChildren())
	{
		if (child->Run())
			return true;
	}

	return false;
}

/**********************************************************/

cSequence::cSequence(ImVec2& position)
	:cCompositeTask("Sequence")
{
	_pos = position;
}

bool cSequence::Run()
{
	for (auto&& child : *GetChildren())
	{
		if (!child->Run())
			return false;
	}

	return true;
}