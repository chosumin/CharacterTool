#include "stdafx.h"
#include "BaseTasks.h"
#include "cBehaviorTree.h"

cTask::cTask(weak_ptr<cBehaviorTree> tree, string name,
			 const ImVec2& position, const weak_ptr<cTask>& parent)
	: _taskName(name)
	, _pos(position)
	, _delete(eTaskState::NONE)
	, _tree(tree)
	, _state(eState::NONE)
	, _parent(parent)
{
}

void cTask::SetInitState(const bool& includeRunning)
{
	//러닝 포함 모두 초기화이거나 러닝상태 아닐 경우 초기화
	if (includeRunning || _state != eState::RUNNING)
			_state = eState::NONE;
	
	if (GetChildren())
	{
		for (auto&& child : *GetChildren())
			child->SetInitState(includeRunning);
	}
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

weak_ptr<cTask> cTask::GetParent() const
{
	return _parent;
}

/**********************************************************/

const char* cCompositeTask::ITEMS[3] = { "Selector", "Sequence", "Parallel" };

cCompositeTask::cCompositeTask(weak_ptr<cBehaviorTree> tree,
							   const ImVec2& position,
							   weak_ptr<cTask> parent)
	: cTask(tree, "Composite Task", position, parent)
	, _type(0)
{
}

cCompositeTask::~cCompositeTask()
{
}

cTask::eState cCompositeTask::Run()
{
	UINT i = 0;
	if (_state == eState::RUNNING)
	{
		for (; i < _children.size(); ++i)
		{
			if (_children[i]->GetRunningState() == eState::RUNNING)
				break;
		}
	}

	switch (_type)
	{
		case 0: //Selector
		{
			for (; i < _children.size(); ++i)
			{
				_state = _children[i]->Run();
				if (_state == eState::SUCCESS || _state == eState::RUNNING)
					return _state;
			}
			
			return _state = eState::FAILURE;
		}
		case 1: //Sequence
		{
			for (; i < _children.size(); ++i)
			{
				_state = _children[i]->Run();
				if (_state == eState::FAILURE || _state == eState::RUNNING)
					return _state;
			}

			return _state = eState::SUCCESS;
		}
		case 2: //Parallel
		{
			UINT successCount = 0;
			for (; i < _children.size(); ++i)
			{
				_state = _children[i]->Run();

				if (_state == eState::FAILURE)
					return _state;

				if (_state == eState::SUCCESS)
					successCount++;
			}

			_state = successCount >= _children.size() ? eState::SUCCESS : eState::RUNNING;

			if (_state == eState::RUNNING)
				_tree.lock()->SetCurrentTask(shared_from_this());

			return _state;
		}
	}

	return eState::NONE;
}

cCompositeTask::TaskList * cCompositeTask::GetChildren()
{
	return &_children;
}

void cCompositeTask::RenderInfo()
{
	ImGui::Text("%s", GetName().c_str());

	ImGui::NewLine();

	ImGui::Combo("", &_type, ITEMS, 3);
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

cRootTask::cRootTask()
	:cCompositeTask(weak_ptr<cBehaviorTree>())
{
	_taskName = "ROOT";
	_type = 0;
}

cRootTask::~cRootTask()
{
}
